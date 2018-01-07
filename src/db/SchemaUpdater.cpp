/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */

#include "db/SchemaUpdater.h"
#include "Db.h"

namespace greenthumb {
namespace db {

void SchemaUpdater::Update() const {
    soci::session session(Db::GetConnectionPool());

    int32_t dbVersion = GetDatabaseVersion();

    if (dbVersion < 1) {
        UpdateToVersion1();
    }
}

const int32_t SchemaUpdater::GetDatabaseVersion() const {
    soci::session session(Db::GetConnectionPool());

    int32_t dbVersion = 0;

    soci::statement st = (session.prepare << "PRAGMA user_version", soci::into(dbVersion));

    st.execute();

    if (st.fetch())  {
        return dbVersion;
    }

    return 0;
}

const void SchemaUpdater::SetDatabaseVersion(const int32_t databaseVersion) const {
    soci::session session(Db::GetConnectionPool());

    session << "PRAGMA user_version = " << databaseVersion;
}

const bool SchemaUpdater::TableExists(const std::string& tableName) const {
    soci::session session(Db::GetConnectionPool());

    std::string rowName;

    soci::statement st = (session.prepare << "SELECT name FROM sqlite_master WHERE type = 'table' AND name = :tableName",
        soci::use(tableName), soci::into(rowName));

    st.execute();

    if (st.fetch())  {
        return true;
    }

    return false;
}

const void SchemaUpdater::UpdateToVersion1() const {
    if (!TableExists("config")) {
        soci::session session(Db::GetConnectionPool());

        session << "CREATE TABLE config ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "config_key CHARACTER VARYING(50) NOT NULL,"
            "config_value CHARACTER VARYING(50) NOT NULL"
        ")";
    }

   if (!TableExists("account_statement_item")) {
        soci::session session(Db::GetConnectionPool());

        session << "CREATE TABLE account_statement_item ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "exchange_id INTEGER NOT NULL,"
            "ref_id VARCHAR NOT NULL,"
            "item_date DATE NOT NULL,"
            "amount DOUBLE PRECISION,"
            "balance DOUBLE PRECISION NOT NULL,"
            "avg_price DOUBLE PRECISION NOT NULL,"
            "bet_size DOUBLE PRECISION NOT NULL,"
            "bet_type VARCHAR(1) NOT NULL,"
            "event_id BIGINT NOT NULL,"
            "event_type_id BIGINT NOT NULL,"
            "full_market_name VARCHAR NOT NULL,"
            "gross_bet_amount DOUBLE PRECISION NOT NULL,"
            "market_name VARCHAR NOT NULL,"
            "placed_date DATE NOT NULL,"
            "selection_id BIGINT,"
            "selection_name VARCHAR,"
            "start_date date NOT NULL,"
            "transaction_type VARCHAR NOT NULL,"
            "win_lose VARCHAR NOT NULL,"
            "total_balance DOUBLE PRECISION)";
    }

    if (!TableExists("account_statement_market")) {
        soci::session session(Db::GetConnectionPool());

        session << "CREATE TABLE account_statement_market ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "exchange_id INTEGER NOT NULL,"
            "item_date DATE,"
            "balance DOUBLE PRECISION NOT NULL,"
            "amount DOUBLE PRECISION NOT NULL,"
            "event_id BIGINT NOT NULL,"
            "full_market_name VARCHAR NOT NULL,"
            "total_balance DOUBLE PRECISION NOT NULL)";
    }

    SetDatabaseVersion(1);
}

}
}
