/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_LISTCURRENTORDERS_H
#define WORKER_LISTCURRENTORDERS_H

#include <greentop/ExchangeApi.h>

#include <wx/event.h>
#include <wx/thread.h>

#include "dialog/Settings.h"
#include "entity/Market.h"
#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(LIST_CURRENT_ORDERS, wxThreadEvent);

/**
 * Retrieves a list of unmatched (or partially unmatched) orders for a market from betfair.
 */
class ListCurrentOrders : public Worker {
    public:

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         * @param market The market whose orders will be retrieved.
         */
        ListCurrentOrders(
            wxEvtHandler* eventHandler,
            const entity::Market& market = entity::Market(),
            const uint32_t currentPage = 1
        );

    protected:
        virtual ExitCode Entry();
    private:

        entity::Market market;
        uint32_t currentPage;

        greentop::CurrentOrderSummaryReport DoListCurrentOrders();
};

}
}

#endif // WORKER_LISTCURRENTORDERS_H
