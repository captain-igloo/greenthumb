/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_GETACCOUNTSTATEMENT_H
#define WORKER_GETACCOUNTSTATEMENT_H

#include <greentop/account/AccountStatementReport.h>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(GET_ACCOUNT_STATEMENT, wxThreadEvent);

/**
 * Get account statement and synchronise with local database.
 */
class GetAccountStatement : public Worker {
    public:

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         */
        GetAccountStatement(wxEvtHandler* eventHandler);

    protected:
        virtual ExitCode Entry();

    private:

        static const unsigned PAGE_SIZE;
        int exchangeId;

        void DoGetAccountStatement();
        bool GetAccountStatementPage(const greentop::GetAccountStatementRequest& gasr);
        void PopulateAccountStatementMarket() const;

};

}
}

#endif // WORKER_GETACCOUNTSTATEMENT_H
