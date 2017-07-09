/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/

#include <wx/log.h>

#include "worker/PlaceOrders.h"

#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(PLACE_ORDERS, wxThreadEvent);

PlaceOrders::PlaceOrders(wxEvtHandler* eventHandler, const greentop::PlaceOrdersRequest& placeOrdersRequest)
    : Worker(eventHandler), placeOrdersRequest(placeOrdersRequest) {
}

wxThread::ExitCode PlaceOrders::Entry() {
    wxLogStatus("Place orders ...");

    try {
        if (DoPlaceOrder()) {
            wxLogStatus("Place orders ... Success");
        } else {
            wxLogStatus("Place orders ... Failed");
        }

    } catch (std::exception const& e) {
        wxLogError("Failed to place orders: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(PLACE_ORDERS);
    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}

bool PlaceOrders::DoPlaceOrder() {

    greentop::PlaceExecutionReport resp =
        GreenThumb::GetBetfairApi().placeOrders(placeOrdersRequest);

    return resp.isSuccess();

}

}
}
