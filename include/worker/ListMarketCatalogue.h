/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_LISTMARKETCATALOGUE_H
#define WORKER_LISTMARKETCATALOGUE_H

#include <wx/event.h>
#include <wx/thread.h>

#include "greentop/betting/MarketCatalogue.h"
#include "worker/Worker.h"
#include "GreenThumbFrame.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(LIST_MARKET_CATALOGUE, wxThreadEvent);

/**
 * Retrieves information about markets from betfair that does not change (or changes very rarely).
 */
class ListMarketCatalogue : public Worker {
    public:
        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         * @param exchange The markets' exchange.
         * @param marketIds The markets whose information to retrieve.
         */
        ListMarketCatalogue(wxEvtHandler* eventHandler, const greentop::Exchange exchange, const std::set<std::string>& marketIds);
    protected:
        virtual ExitCode Entry();
    private:
        greentop::Exchange exchange;
        std::set<std::string> marketIds;
        std::map<std::string, entity::Market> betfairMarkets;

        bool DoListMarketCatalogue(const std::set<std::string>& marketIdsPage);

};

}
}

#endif // WORKER_LISTMARKETCATALOGUE_H
