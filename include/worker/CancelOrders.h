/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_CANCELORDERS_H
#define WORKER_CANCELORDERS_H

#include <greentop/Exchange.h>
#include <greentop/betting/CancelExecutionReport.h>
#include <string>
#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(CANCEL_ORDERS, wxThreadEvent);

/**
 * Cancels the specified order.
 */
class CancelOrders : public Worker {
    public:

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         * @param exchange The order's exchange.
         * @param marketId The order's marketId.
         * @param betId The bet id of the order to cancel.
         */
        CancelOrders(wxEvtHandler* eventHandler, const greentop::Exchange exchange,
            const std::string& marketId, const std::string& betId);

    protected:

        virtual ExitCode Entry();

    private:

        greentop::Exchange exchange;
        std::string marketId;
        std::string betId;

        greentop::CancelExecutionReport DoCancelOrders();
};

}
}

#endif // WORKER_CANCELORDERS_H
