/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */

#include "GreenThumb.h"
#include "entity/Config.h"
#include "worker/GetAccountDetails.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(GET_ACCOUNT_DETAILS, wxThreadEvent);

GetAccountDetails::GetAccountDetails(wxEvtHandler* eventHandler) : Worker(eventHandler) {
    description = "Get account details";
}

wxThread::ExitCode GetAccountDetails::Entry() {
    wxThreadEvent* event = new wxThreadEvent(GET_ACCOUNT_DETAILS);

    try {
        DoGetAccountDetails();
        event->SetString(_(description) + _(" ... Success"));
    } catch (std::exception const& e) {
        event->SetString(_(description) + _(" ... Failed: " + _(e.what())));
    }

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::AccountDetailsResponse GetAccountDetails::DoGetAccountDetails() {

    greentop::AccountDetailsResponse adr = GreenThumb::GetBetfairApi().getAccountDetails(greentop::Exchange::UK);
    if (!TestDestroy() && adr.isSuccess()) {
        entity::Config::SetConfigValue("accountCurrency", adr.getCurrencyCode());
    }

    return adr;
}

}
}
