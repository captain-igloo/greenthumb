/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include "GreenThumb.h"
#include "worker/GetAccountFunds.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(GET_ACCOUNT_FUNDS_UK, wxThreadEvent);
wxDEFINE_EVENT(GET_ACCOUNT_FUNDS_AUS, wxThreadEvent);

GetAccountFunds::GetAccountFunds(wxEvtHandler* eventHandler, const greentop::Wallet& wallet) :
    Worker(eventHandler), wallet(wallet) {

}

wxThread::ExitCode GetAccountFunds::Entry() {

    greentop::AccountFundsResponse afr;

    try {
        afr = DoGetAccountFunds();
    } catch (const std::exception& e) {
        wxLogStatus("Get account funds ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event;

    if (wallet == greentop::Wallet::UK) {
        event = new wxThreadEvent(GET_ACCOUNT_FUNDS_UK);
    } else {
        event = new wxThreadEvent(GET_ACCOUNT_FUNDS_AUS);
    }

    event->SetPayload<greentop::AccountFundsResponse>(afr);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::AccountFundsResponse GetAccountFunds::DoGetAccountFunds() {

    greentop::GetAccountFundsRequest req(wallet);

    greentop::AccountFundsResponse afr = GreenThumb::GetBetfairApi().getAccountFunds(
        greentop::Exchange::UK, req);

    return afr;
}

}
}
