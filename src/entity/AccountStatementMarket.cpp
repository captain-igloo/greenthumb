#include <string>

#include "entity/AccountStatementItem.h"
#include "entity/AccountStatementMarket.h"
#include "Db.h"

namespace greenthumb {
namespace entity {

std::vector<AccountStatementMarket> AccountStatementMarket::FetchAccountStatementMarketPage(uint32_t fromRecord, uint32_t pageSize) {

    std::vector<AccountStatementMarket> ases;
    AccountStatementMarket ase;

    soci::session session(Db::GetConnectionPool());

    soci::statement st = (session.prepare << "SELECT * FROM account_statement_market ORDER BY item_date DESC, id LIMIT :pageSize OFFSET :fromRecord",
            soci::into(ase), soci::use(pageSize), soci::use(fromRecord));

    st.execute();

    while (st.fetch()) {
        ases.push_back(ase);
    }

    return ases;
}

void AccountStatementMarket::Populate() {

    uint32_t exchangeId;
    std::tm itemDate;
    uint64_t eventId;
    std::string fullMarketName;
    double amount;

    soci::session session(Db::GetConnectionPool());
    AccountStatementItem accountStatementItem;

    std::string query = "SELECT "
            "asi.exchange_id,"
            "asi.item_date,"
            "asi.event_id,"
            "asi.full_market_name,"
            "ROUND(SUM(asi.amount), 2) AS amount "
        "FROM "
            "account_statement_item AS asi "
        "LEFT JOIN "
            "account_statement_market AS ase "
        "ON "
            "asi.event_id = ase.event_id "
        "WHERE "
            "ase.event_id IS NULL "
        "GROUP BY "
            "asi.exchange_id,"
            "asi.item_date,"
            "asi.event_id,"
            "asi.full_market_name "
        "ORDER BY "
            "asi.item_date";

    soci::statement st = (session.prepare << query, soci::into(exchangeId),
        soci::into(itemDate), soci::into(eventId), soci::into(fullMarketName), soci::into(amount));
    st.execute();

    std::vector<AccountStatementMarket> ases;

    while (st.fetch()) {

        AccountStatementMarket accountStatementEvent;
        accountStatementEvent.exchangeId = exchangeId;
        accountStatementEvent.itemDate = itemDate;
        accountStatementEvent.balance = 0;
        accountStatementEvent.amount = amount;
        accountStatementEvent.eventId = eventId;
        accountStatementEvent.fullMarketName = fullMarketName;
        accountStatementEvent.totalBalance = AccountStatementItem::GetTotalBalanceForEvent(eventId);
        ases.push_back(accountStatementEvent);

    }

    for (unsigned i = 0; i < ases.size(); ++i) {
        ases[i].Insert();
    }

}

void AccountStatementMarket::Insert() {

    soci::session session(Db::GetConnectionPool());

    session << "INSERT INTO account_statement_market ("
        "exchange_id, item_date, balance, amount, event_id, full_market_name, total_balance) VALUES ("
        ":exchangeId,  :itemDate, :balance, :amount, :eventId, :fullMarketName, :totalBalance)",
        soci::use(exchangeId), soci::use(itemDate), soci::use(balance), soci::use(amount),
        soci::use(eventId), soci::use(fullMarketName), soci::use(totalBalance);

}

uint32_t AccountStatementMarket::GetNumberRows() {

    soci::session session(Db::GetConnectionPool());

    uint32_t numberRows = 0;

    std::string query = "SELECT count(*) FROM account_statement_market";

    session << query, soci::into(numberRows);

    return numberRows;

}

void AccountStatementMarket::SetId(const uint64_t id) {
    this->id = id;
}
const uint64_t AccountStatementMarket::GetId() const {
    return id;
}

void AccountStatementMarket::SetExchangeId(const uint32_t exchangeId) {
    this->exchangeId = exchangeId;
}
const uint32_t AccountStatementMarket::GetExchangeId() const {
    return exchangeId;
}

void AccountStatementMarket::SetItemDate(const std::tm& itemDate) {
    this->itemDate = itemDate;
}
const std::tm& AccountStatementMarket::GetItemDate() const {
    return itemDate;
}

void AccountStatementMarket::SetBalance(const double balance) {
    this->balance = balance;
}
const double AccountStatementMarket::GetBalance() const {
    return balance;
}

void AccountStatementMarket::SetAmount(const double amount) {
    this->amount = amount;
}
const double AccountStatementMarket::GetAmount() const {
    return amount;
}

void AccountStatementMarket::SetEventId(const uint64_t eventId) {
    this->eventId = eventId;
}
const uint64_t AccountStatementMarket::GetEventId() const {
    return eventId;
}

void AccountStatementMarket::SetFullMarketName(const std::string& fullMarketName) {
    this->fullMarketName = fullMarketName;
}
const std::string& AccountStatementMarket::GetFullMarketName() const {
    return fullMarketName;
}

void AccountStatementMarket::SetTotalBalance(const double totalBalance) {
    this->totalBalance = totalBalance;
}
const double AccountStatementMarket::GetTotalBalance() const {
    return totalBalance;
}


}
}
