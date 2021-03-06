/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include <vector>

#include "worker/CancelOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(CANCEL_ORDERS, wxThreadEvent);

CancelOrders::CancelOrders(
    wxEvtHandler* eventHandler,
    const std::string& marketId,
    const std::string& betId) :
    Worker(eventHandler),
    marketId(marketId),
    betId(betId) {
}

wxThread::ExitCode CancelOrders::Entry() {
    wxLogStatus("Cancel order ...");

    greentop::sport::CancelExecutionReport cer;

    try {
        cer = DoCancelOrders();
        wxLogStatus("Cancel order ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Cancel order ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(CANCEL_ORDERS);
    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<greentop::sport::CancelExecutionReport>(cer);

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::sport::CancelExecutionReport CancelOrders::DoCancelOrders() {

    std::vector<greentop::sport::CancelInstruction> instructions;
    greentop::sport::CancelInstruction ci(betId);
    instructions.push_back(ci);

    greentop::sport::CancelOrdersRequest cor(marketId, instructions);
    return GreenThumb::GetBetfairApi().cancelOrders(cor);

}

}
}
