/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_LISTMARKETBOOK_H
#define WORKER_LISTMARKETBOOK_H

#include <greentop/sport/MarketCatalogue.h>
#include <wx/event.h>
#include <wx/thread.h>

#include "entity/Market.h"
#include "Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(LIST_MARKET_BOOK, wxThreadEvent);

/**
 * Retrieve dynamic data about a market from betfair.
 */
class ListMarketBook : public Worker {
    public:

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         * @param market The market whose data to retrieve.
         */
        ListMarketBook(wxEvtHandler* eventHandler, const entity::Market& market);

    protected:
        virtual ExitCode Entry();

    private:

        entity::Market market;
        greentop::sport::MarketBook betfairMarketBook;

        bool DoListMarketBook();

};

}
}

#endif // WORKER_LISTMARKETBOOK_H
