#ifndef WORKER_PLACEORDERS_H
#define WORKER_PLACEORDERS_H

#include <greentop/ExchangeApi.h>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(PLACE_ORDERS, wxThreadEvent);

class PlaceOrders : public Worker {
    public:

        PlaceOrders(wxEvtHandler* eventHandler, const greentop::Exchange exchange,
            const greentop::PlaceOrdersRequest& placeOrdersRequest);

    protected:

        virtual ExitCode Entry();

    private:

        greentop::Exchange exchange;
        greentop::PlaceOrdersRequest placeOrdersRequest;

        bool DoPlaceOrder();

};

}
}

#endif // WORKER_PLACEORDERS_H
