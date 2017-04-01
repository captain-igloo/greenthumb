/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/

#include <wx/log.h>

#include "worker/PlaceOrders.h"

#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(PLACE_ORDERS, wxThreadEvent);

PlaceOrders::PlaceOrders(wxEvtHandler* eventHandler, const greentop::Exchange exchange,
    const greentop::PlaceOrdersRequest& placeOrdersRequest)
    : Worker(eventHandler), exchange(exchange), placeOrdersRequest(placeOrdersRequest) {
    description = "Place orders";
}

wxThread::ExitCode PlaceOrders::Entry() {
    wxThreadEvent* event = new wxThreadEvent(PLACE_ORDERS);

    try {
        if (DoPlaceOrder()) {
            event->SetString(_(description) + _(" ... Success"));
        } else {
            event->SetString(_(description) + _(" ... Failed"));
        }

    } catch (std::exception const& e) {
        wxLogError("Failed to place orders: " + _(e.what()));
    }

    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}

bool PlaceOrders::DoPlaceOrder() {

    greentop::PlaceExecutionReport resp =
        GreenThumb::GetBetfairApi().placeOrders(exchange, placeOrdersRequest);

    return resp.isSuccess();

}

}
}
