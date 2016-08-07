#ifndef WORKER_TRANSFERFUNDS_H
#define WORKER_TRANSFERFUNDS_H

#include <wx/event.h>
#include <wx/thread.h>

#include <greentop/ExchangeApi.h>

#include "Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(TRANSFER_FUNDS, wxThreadEvent);

class TransferFunds : public Worker {
    public:

        TransferFunds(wxEvtHandler* eventHandler, const greentop::TransferFundsRequest& transferFundsRequest);

    protected:
        virtual ExitCode Entry();

    private:

        greentop::TransferFundsRequest transferFundsRequest;

        void DoTransferFunds();
};

}
}

#endif // WORKER_TRANSFERFUNDS_H
