/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include <greentop/Exchange.h>

#include "entity/AccountStatementItem.h"
#include "Db.h"

namespace greenthumb {
namespace entity {

std::tm AccountStatementItem::GetMostRecentItemDate() {

    std::tm mostRecent = std::tm();
    AccountStatementItem accountStatementItem;

    soci::session session(Db::GetConnectionPool());

    std::string query = "SELECT * FROM account_statement_item ORDER BY item_date DESC LIMIT 1";

    soci::statement st = (session.prepare << query, soci::into(accountStatementItem));

    st.execute();

    if (st.fetch()) {
        mostRecent = accountStatementItem.itemDate;
    }

    return mostRecent;

}

std::vector<AccountStatementItem> AccountStatementItem::FetchAccountStatementItemPage(uint32_t fromRecord, uint32_t pageSize) {

    std::vector<AccountStatementItem> asis;
    AccountStatementItem asi;

    soci::session session(Db::GetConnectionPool());

    soci::statement st = (session.prepare << "SELECT * FROM account_statement_item ORDER BY id DESC LIMIT :pageSize OFFSET :fromRecord",
            soci::into(asi), soci::use(pageSize), soci::use(fromRecord));

    st.execute();

    while (st.fetch()) {
        asis.push_back(asi);
    }

    return asis;
}

std::vector<AccountStatementItem> AccountStatementItem::FetchTotalBalanceNull() {

    std::vector<AccountStatementItem> asis;
    AccountStatementItem asi;

    soci::session session(Db::GetConnectionPool());

    soci::statement st = (session.prepare << "SELECT * FROM account_statement_item WHERE total_balance IS NULL", soci::into(asi));

    st.execute();

    while (st.fetch()) {
        asis.push_back(asi);
    }

    return asis;

}

double AccountStatementItem::GetTotalBalanceForEvent(uint64_t eventId) {

    double totalBalanceForEvent;

    soci::session session(Db::GetConnectionPool());

    std::string query = "SELECT total_balance FROM account_statement_item where event_id = :eventId ORDER BY id DESC LIMIT 1";

    session << query, soci::into(totalBalanceForEvent), soci::use(eventId);

    return totalBalanceForEvent;
}

double AccountStatementItem::GetPreviousItemBalance() {

    double previousItemBalance = 0;

    AccountStatementItem accountStatementItem;

    soci::session session(Db::GetConnectionPool());

    uint32_t otherExchangeId;

    if (exchangeId == greentop::Exchange::UK) {
        otherExchangeId = greentop::Exchange::AUS;
    } else {
        otherExchangeId = greentop::Exchange::UK;
    }

    std::string query = "SELECT * FROM account_statement_item WHERE exchange_id = :exchangeId AND "
        "(item_date < :itemDate1 OR (item_date = :itemDate2 AND id > :id)) ORDER BY item_date DESC, id LIMIT 1";

    soci::statement st = (session.prepare << query, soci::into(accountStatementItem),
        soci::use(otherExchangeId), soci::use(itemDate), soci::use(itemDate), soci::use(id));

    st.execute();

    if (st.fetch()) {
        previousItemBalance = accountStatementItem.balance;
    }

    return previousItemBalance;

}

void AccountStatementItem::Update() {

    soci::session session(Db::GetConnectionPool());

    totalBalance = GetPreviousItemBalance() + balance;

    session << "UPDATE account_statement_item SET total_balance = :total_balance WHERE id = :id",
        soci::use(totalBalance), soci::use(id);

}

bool AccountStatementItem::Insert() {
    soci::session session(Db::GetConnectionPool());

    session << "INSERT INTO account_statement_item ("
        "exchange_id, ref_id, item_date, amount, balance, avg_price, bet_size, bet_type, event_id, event_type_id, full_market_name, "
        "gross_bet_amount, market_name, placed_date, selection_id, selection_name, start_date,"
        "transaction_type, win_lose) VALUES ("
        ":exchange_id, :ref_id, :item_date, :amount, :balance, :avg_price, :bet_size, :bet_type, :event_id, :event_type_id, :full_market_name, "
        ":gross_bet_amount, :market_name, :placed_date, :selection_id, :selection_name, :start_date, "
        ":transaction_type, :win_lose)", soci::use(exchangeId), soci::use(refId), soci::use(itemDate), soci::use(amount),
        soci::use(balance), soci::use(avgPrice), soci::use(betSize), soci::use(betType), soci::use(eventId),
        soci::use(eventTypeId), soci::use(fullMarketName), soci::use(grossBetAmount),
        soci::use(marketName), soci::use(placedDate), soci::use(selectionId),
        soci::use(selectionName), soci::use(startDate), soci::use(transactionType), soci::use(winLose);

    return true;
}

uint32_t AccountStatementItem::GetNumberRows() {

    soci::session session(Db::GetConnectionPool());

    uint32_t numberRows = 0;

    std::string query = "SELECT count(*) FROM account_statement_item";

    session << query, soci::into(numberRows);

    return numberRows;

}

void AccountStatementItem::SetId(const uint64_t id) {
    this->id = id;
}
const uint64_t AccountStatementItem::GetId() const {
    return id;
}

void AccountStatementItem::SetExchangeId(const uint32_t exchangeId) {
    this->exchangeId = exchangeId;
}
const uint32_t AccountStatementItem::GetExchangeId() const {
    return exchangeId;
}

void AccountStatementItem::SetRefId(const std::string& refId) {
    this->refId = refId;
}
const std::string& AccountStatementItem::GetRefId() const {
    return refId;
}

void AccountStatementItem::SetItemDate(const std::tm& itemDate) {
    this->itemDate = itemDate;
}
const std::tm& AccountStatementItem::GetItemDate() const {
    return itemDate;
}

void AccountStatementItem::SetAmount(const double amount) {
    this->amount = amount;
}
const double AccountStatementItem::GetAmount() const {
    return amount;
}

void AccountStatementItem::SetBalance(const double balance) {
    this->balance = balance;
}
const double AccountStatementItem::GetBalance() const {
    return balance;
}

void AccountStatementItem::SetAvgPrice(const double avgPrice) {
    this->avgPrice = avgPrice;
}
const double AccountStatementItem::GetAvgPrice() const {
    return avgPrice;
}

void AccountStatementItem::SetBetSize(const double betSize) {
    this->betSize = betSize;
}
const double AccountStatementItem::GetBetSize() const {
    return betSize;
}

void AccountStatementItem::SetBetType(const std::string& betType) {
    this->betType = betType;
}
const std::string& AccountStatementItem::GetBetType() const {
    return betType;
}

void AccountStatementItem::SetEventId(const uint64_t eventId) {
    this->eventId = eventId;
}
const uint64_t AccountStatementItem::GetEventId() const {
    return eventId;
}

void AccountStatementItem::SetEventTypeId(const uint64_t eventTypeId) {
    this->eventTypeId = eventTypeId;
}
const uint64_t AccountStatementItem::GetEventTypeId() const {
    return eventTypeId;
}

void AccountStatementItem::SetFullMarketName(const std::string& fullMarketName) {
    this->fullMarketName = fullMarketName;
}
const std::string& AccountStatementItem::GetFullMarketName() const {
    return fullMarketName;
}

void AccountStatementItem::SetGrossBetAmount(const double grossBetAmount) {
    this->grossBetAmount = grossBetAmount;
}
const double AccountStatementItem::GetGrossBetAmount() const {
    return grossBetAmount;
}

void AccountStatementItem::SetMarketName(const std::string& marketName) {
    this->marketName = marketName;
}
const std::string& AccountStatementItem::GetMarketName() const {
    return marketName;
}

void AccountStatementItem::SetPlacedDate(const std::tm& placedDate) {
    this->placedDate = placedDate;
}
const std::tm& AccountStatementItem::GetPlacedDate() const {
    return placedDate;
}

void AccountStatementItem::SetSelectionId(const uint64_t selectionId) {
    this->selectionId = selectionId;
}
const uint64_t AccountStatementItem::GetSelectionId() const {
    return selectionId;
}

void AccountStatementItem::SetSelectionName(const std::string& selectionName) {
    this->selectionName = selectionName;
}
const std::string& AccountStatementItem::GetSelectionName() const {
    return selectionName;
}

void AccountStatementItem::SetStartDate(const std::tm& startDate) {
    this->startDate = startDate;
}
const std::tm& AccountStatementItem::GetStartDate() const {
    return startDate;
}

void AccountStatementItem::SetTransactionType(const std::string& transactionType) {
    this->transactionType = transactionType;
}
const std::string& AccountStatementItem::GetTransactionType() const {
    return transactionType;
}

void AccountStatementItem::SetWinLose(const std::string& winLose) {
    this->winLose = winLose;
}
const std::string& AccountStatementItem::GetWinLose() const {
    return winLose;
}

void AccountStatementItem::SetTotalBalance(const double totalBalance) {
    this->totalBalance = totalBalance;
}
const double AccountStatementItem::GetTotalBalance() const {
    return totalBalance;
}

}
}
