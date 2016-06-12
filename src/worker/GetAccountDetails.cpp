/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include "GreenThumb.h"
#include "entity/Config.h"
#include "worker/GetAccountDetails.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(GET_ACCOUNT_DETAILS, wxThreadEvent);

GetAccountDetails::GetAccountDetails(wxEvtHandler* eventHandler) : Worker(eventHandler) {
}

wxThread::ExitCode GetAccountDetails::Entry() {

    wxLogStatus("Get account details ...");

    try {
        DoGetAccountDetails();
        wxLogStatus("Get account details ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Get account details ... Failed: " + _(e.what()));
    }

    QueueEvent(GET_ACCOUNT_DETAILS);

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
