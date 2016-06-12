#include "Db.h"

namespace greenthumb {

soci::connection_pool& Db::GetConnectionPool() {

    static soci::connection_pool pool(poolSize);
    return pool;
}

void Db::InitialisePool() {
    soci::connection_pool& pool = GetConnectionPool();

    wxStandardPaths standardPaths = wxStandardPaths::Get();
    wxString dbFilename = standardPaths.GetUserDataDir();

    if (!wxDirExists(dbFilename)) {
        wxMkdir(dbFilename);
    }

    dbFilename += wxFileName::GetPathSeparator() + _("db.sqlite3");

    for (unsigned int i = 0; i < poolSize; ++i) {
        soci::session& sql = pool.at(i);
        sql.open(soci::sqlite3, "dbname=" + dbFilename.ToStdString() + " timeout=30");
    }

    CreateTables();
}

void Db::CreateTables() {

    std::string create;

    if (!TableExists("config")) {

        soci::session session(GetConnectionPool());

        create = "CREATE TABLE config ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "config_key CHARACTER VARYING(50) NOT NULL,"
            "config_value CHARACTER VARYING(50) NOT NULL"
        ")";

        session << create;
    }

    if (!TableExists("exchange")) {

        soci::session session(GetConnectionPool());

        create = "CREATE TABLE exchange ("
            "id INTEGER NOT NULL PRIMARY KEY,"
            "exchange CHARACTER VARYING(8) NOT NULL"
        ")";

        session << create;

        session << "INSERT INTO exchange VALUES (1, 'UK')";
        session << "INSERT INTO exchange VALUES (2, 'AUS')";

    }

    if (!TableExists("account_statement_item")) {

        soci::session session(GetConnectionPool());

        create = "create table account_statement_item ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "exchange_id INTEGER NOT NULL REFERENCES exchange(id),"
            "ref_id varchar not null,"
            "item_date date not null,"
            "amount double precision,"
            "balance double precision not null,"
            "avg_price double precision not null,"
            "bet_size double precision not null,"
            "bet_type varchar(1) not null,"
            "event_id bigint not null,"
            "event_type_id bigint not null,"
            "full_market_name varchar not null,"
            "gross_bet_amount double precision not null,"
            "market_name varchar not null,"
            "placed_date date not null,"
            "selection_id bigint,"
            "selection_name varchar,"
            "start_date date not null,"
            "transaction_type varchar not null,"
            "win_lose varchar not null,"
            "total_balance double precision)";

        session << create;
    }

    if (!TableExists("account_statement_market")) {

        soci::session session(GetConnectionPool());

        create = "CREATE TABLE account_statement_market ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "exchange_id INTEGER NOT NULL REFERENCES exchange(id),"
            "item_date date,"
            "balance double precision NOT NULL,"
            "amount double precision NOT NULL,"
            "event_id bigint NOT NULL,"
            "full_market_name varchar NOT NULL,"
            "total_balance double precision NOT NULL)";

        session << create;
    }

}

bool Db::TableExists(const std::string& tableName) {

    soci::session session(GetConnectionPool());

    std::string rowName;

    soci::statement st = (session.prepare << "SELECT name FROM sqlite_master WHERE type = 'table' AND name = :tableName",
        soci::use(tableName), soci::into(rowName));

    st.execute();

    if (st.fetch())  {
        return true;
    }

    return false;
}

}
