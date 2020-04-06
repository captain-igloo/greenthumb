/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_CANCELORDERS_H
#define WORKER_CANCELORDERS_H

#include <greentop/sport/CancelExecutionReport.h>
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
        CancelOrders(wxEvtHandler* eventHandler, const std::string& marketId,
            const std::string& betId);

    protected:

        virtual ExitCode Entry();

    private:
        std::string marketId;
        std::string betId;

        greentop::sport::CancelExecutionReport DoCancelOrders();
};

}
}

#endif // WORKER_CANCELORDERS_H
