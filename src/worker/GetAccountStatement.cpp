/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/log.h>
#include <greentop/ExchangeApi.h>

#include "entity/AccountStatementMarket.h"
#include "entity/AccountStatementItem.h"
#include "worker/GetAccountStatement.h"

namespace greenthumb {
namespace worker {

const unsigned GetAccountStatement::PAGE_SIZE = 100;

wxDEFINE_EVENT(GET_ACCOUNT_STATEMENT, wxThreadEvent);

GetAccountStatement::GetAccountStatement(wxEvtHandler* eventHandler) :
    Worker(eventHandler) {

}

wxThread::ExitCode GetAccountStatement::Entry() {

    wxLogStatus("Retrieving account statement ...");
    try {
        DoGetAccountStatement();
        wxLogStatus("Retrieving account statement ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Retrieving account statement ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(GET_ACCOUNT_STATEMENT);
    event->ResumePropagation(1);

    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}

void GetAccountStatement::DoGetAccountStatement() {

    uint32_t fromRecord = 0;
    greentop::GetAccountStatementRequest gasr;
    std::tm fromDate;
    fromDate = entity::AccountStatementItem::GetMostRecentItemDate();

    fromDate.tm_sec++;
    greentop::TimeRange timeRange(fromDate);
    gasr.setItemDateRange(timeRange);
    gasr.setWallet(greentop::Wallet::UK);
    gasr.setFromRecord(fromRecord);
    gasr.setRecordCount(PAGE_SIZE);

    fromRecord = 0;
    gasr.setFromRecord(fromRecord);

    // UK wallet
    while (GetAccountStatementPage(gasr)) {
        fromRecord += PAGE_SIZE;
        gasr.setFromRecord(fromRecord);
    }

    if (TestDestroy()) {
        return;
    }

    std::vector<entity::AccountStatementItem> asis = entity::AccountStatementItem::FetchTotalBalanceNull();
    for (uint32_t i = 0; i < asis.size(); ++i) {
        asis[i].Update();
    }

    PopulateAccountStatementMarket();

}

bool GetAccountStatement::GetAccountStatementPage(const greentop::GetAccountStatementRequest& gasr) {

    greentop::AccountStatementReport asr = GreenThumb::GetBetfairApi().getAccountStatement(
        greentop::Exchange::UK, gasr);

    if (TestDestroy()) {
        return false;
    }

    if (asr.isSuccess()) {

        std::vector<greentop::StatementItem> items = asr.getAccountStatement();

        for (unsigned i = 0; i < items.size(); ++i) {

            greentop::StatementLegacyData data = items[i].getLegacyData();

            entity::AccountStatementItem asi;
            if (gasr.getWallet() == greentop::Wallet::AUSTRALIAN) {
                asi.SetExchangeId(static_cast<int>(greentop::Exchange::AUS));
            } else {
                asi.SetExchangeId(static_cast<int>(greentop::Exchange::UK));
            }
            asi.SetRefId(items[i].getRefId());
            asi.SetItemDate(items[i].getItemDate());
            asi.SetAmount(items[i].getAmount().isValid() ? items[i].getAmount().getValue() : 0);
            asi.SetBalance(items[i].getBalance().isValid() ? items[i].getBalance().getValue() : 0);
            asi.SetAvgPrice(data.getAvgPrice().isValid() ? data.getAvgPrice().getValue() : 0);
            asi.SetBetSize(data.getBetSize().isValid() ? data.getBetSize().getValue() : 0);
            asi.SetBetType(data.getBetType());
            asi.SetEventId(data.getEventId());
            asi.SetEventTypeId(data.getEventTypeId());
            asi.SetFullMarketName(data.getFullMarketName());
            asi.SetGrossBetAmount(data.getGrossBetAmount().isValid() ? data.getGrossBetAmount().getValue() : 0);
            asi.SetMarketName(data.getMarketName());
            asi.SetPlacedDate(data.getPlacedDate());
            asi.SetSelectionId(data.getSelectionId());
            asi.SetSelectionName(data.getSelectionName());
            asi.SetStartDate(data.getStartDate());
            asi.SetTransactionType(data.getTransactionType());
            asi.SetWinLose(data.getWinLose());
            asi.Insert();

        }

    } else {
        std::string errorString = "Failed to get account statement.\nFault code: "
            + asr.getFaultCode() + "\nFault string: " + asr.getFaultString();
        wxLogError(errorString.c_str());
        return false;
    }

    return asr.getMoreAvailable();
}

void GetAccountStatement::PopulateAccountStatementMarket() const {
    entity::AccountStatementMarket::Populate();
}

}
}
