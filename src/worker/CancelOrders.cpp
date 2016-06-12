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

}

wxThread::ExitCode CancelOrders::Entry() {

    wxLogStatus("Cancel order ...");

    greentop::CancelExecutionReport cer;

    try {
        cer = DoCancelOrders();
        wxLogStatus("Cancel order ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Cancel order ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(CANCEL_ORDERS);
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
