/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#ifndef WORKER_GETPRICEHISTORYGRAPH_H
#define WORKER_GETPRICEHISTORYGRAPH_H

#include <greentop/sport/Runner.h>
#include <wx/event.h>
#include <wx/thread.h>

#include "entity/Market.h"
#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(GET_PRICE_HISTORY_GRAPH, wxThreadEvent);

class GetPriceHistoryGraph : public Worker {
    public:

        GetPriceHistoryGraph(
            wxEvtHandler* eventHandler,
            const entity::Market& market,
            const greentop::Runner& runner
        );

    protected:

        virtual ExitCode Entry();

    private:
        entity::Market market;
        greentop::Runner runner;
        double lastPriceTraded;
};

}
}

#endif // WORKER_GETPRICEHISTORYGRAPH_H
