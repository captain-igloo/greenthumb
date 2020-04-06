/**
* Copyright 2020 Colin Doig.  Distributed under the MIT license.
*/
#ifndef WORKER_PLACEORDERS_H
#define WORKER_PLACEORDERS_H

#include <wx/event.h>
#include <wx/thread.h>

#include <greentop/ExchangeApi.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(PLACE_ORDERS, wxThreadEvent);

class PlaceOrders : public Worker {
    public:

        PlaceOrders(wxEvtHandler* eventHandler, const greentop::sport::PlaceOrdersRequest& placeOrdersRequest);

    protected:

        virtual ExitCode Entry();

    private:
        greentop::sport::PlaceOrdersRequest placeOrdersRequest;

        bool DoPlaceOrder();

};

}
}

#endif // WORKER_PLACEORDERS_H
