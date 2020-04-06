/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include "worker/ReplaceOrders.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(REPLACE_ORDERS, wxThreadEvent);

ReplaceOrders::ReplaceOrders(
    wxEvtHandler* eventHandler,
    const std::string& marketId,
    const std::string& betId,
    const double newPrice) :
    Worker(eventHandler),
    marketId(marketId),
    betId(betId),
    newPrice(newPrice) {
}

wxThread::ExitCode ReplaceOrders::Entry() {
    greentop::sport::ReplaceExecutionReport report;

    wxLogStatus("Replace orders ...");
    try {
        report = DoReplaceOrders();
        wxLogStatus("Replace orders ... Success");
    } catch (std::exception const& e) {
        wxLogStatus("Replace orders ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(REPLACE_ORDERS);
    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<greentop::sport::ReplaceExecutionReport>(report);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

greentop::sport::ReplaceExecutionReport ReplaceOrders::DoReplaceOrders() {

    std::vector<greentop::sport::ReplaceInstruction> instructions;

    greentop::sport::ReplaceInstruction instruction;
    instruction.setBetId(betId);
    instruction.setNewPrice(newPrice);

    instructions.push_back(instruction);

    greentop::sport::ReplaceOrdersRequest replaceOrdersRequest(marketId, instructions);

    return GreenThumb::GetBetfairApi().replaceOrders(replaceOrdersRequest);
}

}
}
