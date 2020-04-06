/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_GETACCOUNTDETAILS_H
#define WORKER_GETACCOUNTDETAILS_H

#include <greentop/ExchangeApi.h>
#include <wx/event.h>
#include <wx/thread.h>
#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(GET_ACCOUNT_DETAILS, wxThreadEvent);

/**
 * Retrieves the details relating your betfair account, including your discount rate and Betfair point balance.
 */
class GetAccountDetails : public Worker {
    public:
        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         */
        GetAccountDetails(wxEvtHandler* eventHandler);
    protected:
        virtual ExitCode Entry();
    private:
        greentop::account::AccountDetailsResponse DoGetAccountDetails();
};

}
}

#endif // WORKER_GETACCOUNTDETAILS_H
