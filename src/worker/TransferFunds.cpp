/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include "worker/TransferFunds.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(TRANSFER_FUNDS, wxThreadEvent);

TransferFunds::TransferFunds(wxEvtHandler* eventHandler, const greentop::TransferFundsRequest& transferFundsRequest) :
    Worker(eventHandler),  transferFundsRequest(transferFundsRequest) {

}

wxThread::ExitCode TransferFunds::Entry() {

    wxLogStatus("Transfer funds ...");

    try {
        DoTransferFunds();
        wxLogStatus("Transfer funds ... Success");
    } catch (const std::exception& e) {
        wxLogStatus("Transfer funds failed: " + _(e.what()));
    }

    QueueEvent(TRANSFER_FUNDS);

    return (wxThread::ExitCode) 0;

}

void TransferFunds::DoTransferFunds() {

    if (transferFundsRequest.isValid()) {

        greentop::Exchange exchange = greentop::Exchange::UK;

        greentop::TransferResponse resp = GreenThumb::GetBetfairApi().transferFunds(exchange, transferFundsRequest);

    }
}

}
}
