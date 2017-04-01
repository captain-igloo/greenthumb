/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */

#include <vector>

#include "worker/CancelOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(CANCEL_ORDERS, wxThreadEvent);

CancelOrders::CancelOrders(
    wxEvtHandler* eventHandler,
    const greentop::Exchange exchange,
    const std::string& marketId,
    const std::string& betId) :
    Worker(eventHandler),
    exchange(exchange),
    marketId(marketId),
    betId(betId) {
    description = "Cancel order";
}

wxThread::ExitCode CancelOrders::Entry() {
    wxThreadEvent* event = new wxThreadEvent(CANCEL_ORDERS);

    greentop::CancelExecutionReport cer;

    try {
        cer = DoCancelOrders();
        event->SetString(_(description) + _(" ... Success"));
    } catch (std::exception const& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<greentop::CancelExecutionReport>(cer);

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::CancelExecutionReport CancelOrders::DoCancelOrders() {

    std::vector<greentop::CancelInstruction> instructions;
    greentop::CancelInstruction ci(betId);
    instructions.push_back(ci);

    greentop::CancelOrdersRequest cor(marketId, instructions);
    return GreenThumb::GetBetfairApi().cancelOrders(exchange, cor);

}

}
}
