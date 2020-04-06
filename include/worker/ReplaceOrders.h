/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_REPLACEORDERS_H
#define WORKER_REPLACEORDERS_H

#include <greentop/sport/ReplaceExecutionReport.h>
#include <greentop/Exchange.h>
#include <string>
#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(REPLACE_ORDERS, wxThreadEvent);

class ReplaceOrders : public Worker {
    public:
        ReplaceOrders(wxEvtHandler* eventHandler, const std::string& marketId,
            const std::string& betId, const double newPrice);
    protected:
        virtual ExitCode Entry();
    private:
        std::string marketId;
        std::string betId;
        double newPrice;

        greentop::sport::ReplaceExecutionReport DoReplaceOrders();
};

}
}

#endif // WORKER_REPLACEORDERS_H
