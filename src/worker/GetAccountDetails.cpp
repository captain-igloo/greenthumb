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
}

wxThread::ExitCode GetAccountDetails::Entry() {
    wxLogStatus("Get account details ...");

    try {
        DoGetAccountDetails();
        wxLogStatus("Get account details ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Get account details ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(GET_ACCOUNT_DETAILS);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::AccountDetailsResponse GetAccountDetails::DoGetAccountDetails() {

    greentop::AccountDetailsResponse adr = GreenThumb::GetBetfairApi().getAccountDetails();
    if (!TestDestroy() && adr.isSuccess()) {
        wxString currencyCode(adr.getCurrencyCode().c_str(), wxConvUTF8);
        entity::Config::SetConfigValue("accountCurrency", currencyCode);
    }

    return adr;
}

}
}
