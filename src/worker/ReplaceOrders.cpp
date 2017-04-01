#include "worker/ReplaceOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(REPLACE_ORDERS, wxThreadEvent);

ReplaceOrders::ReplaceOrders(
    wxEvtHandler* eventHandler,
    const greentop::Exchange exchange,
    const std::string& marketId,
    const std::string& betId,
    const double newPrice) :
    Worker(eventHandler),
    exchange(exchange),
    marketId(marketId),
    betId(betId),
    newPrice(newPrice) {
    description = "Replace orders";
}

wxThread::ExitCode ReplaceOrders::Entry() {
    wxThreadEvent* event = new wxThreadEvent(REPLACE_ORDERS);
    greentop::ReplaceExecutionReport report;

    try {
        report = DoReplaceOrders();
        event->SetString(_(description) + _(" ... Success"));
    } catch (std::exception const& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<greentop::ReplaceExecutionReport>(report);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::ReplaceExecutionReport ReplaceOrders::DoReplaceOrders() {

    std::vector<greentop::ReplaceInstruction> instructions;

    greentop::ReplaceInstruction instruction;
    instruction.setBetId(betId);
    instruction.setNewPrice(newPrice);

    instructions.push_back(instruction);

    greentop::ReplaceOrdersRequest replaceOrdersRequest(marketId, instructions);

    return GreenThumb::GetBetfairApi().replaceOrders(exchange, replaceOrdersRequest);
}

}
}
