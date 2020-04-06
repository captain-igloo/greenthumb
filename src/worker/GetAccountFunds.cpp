/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include "GreenThumb.h"
#include "worker/GetAccountFunds.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(GET_ACCOUNT_FUNDS, wxThreadEvent);

GetAccountFunds::GetAccountFunds(wxEvtHandler* eventHandler) : Worker(eventHandler) {
}

wxThread::ExitCode GetAccountFunds::Entry() {
    wxThreadEvent* event;

    event = new wxThreadEvent(GET_ACCOUNT_FUNDS);

    greentop::account::AccountFundsResponse afr;

    wxLogStatus("Get account funds ...");
    try {
        afr = DoGetAccountFunds();
        wxLogStatus("Get account funds ... Success");
    } catch (const std::exception& e) {
        wxLogStatus("Get account funds ... Failed: " + _(e.what()));
    }

    event->SetPayload<greentop::account::AccountFundsResponse>(afr);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::account::AccountFundsResponse GetAccountFunds::DoGetAccountFunds() {

    greentop::account::GetAccountFundsRequest req;

    greentop::account::AccountFundsResponse afr = GreenThumb::GetBetfairApi().getAccountFunds(req);

    return afr;
}

}
}
