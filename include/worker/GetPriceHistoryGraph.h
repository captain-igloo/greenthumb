/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
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
            const greentop::sport::Runner& runner
        );

    protected:

        virtual ExitCode Entry();

    private:
        entity::Market market;
        greentop::sport::Runner runner;
};

}
}

#endif // WORKER_GETPRICEHISTORYGRAPH_H
