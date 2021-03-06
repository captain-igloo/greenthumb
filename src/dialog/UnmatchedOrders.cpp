/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include "dialog/UnmatchedOrders.h"
#include "worker/CancelOrders.h"
#include "worker/ReplaceOrders.h"
#include "UnmatchedOrder.h"

namespace greenthumb {
namespace dialog {

UnmatchedOrders::UnmatchedOrders(wxWindow* parent) : CurrentOrdersPage(parent) {
    if (noOrdersMessage != NULL) {
        noOrdersMessage->SetLabel("There are no unmatched orders.");
    }
    Bind(worker::CANCEL_ORDERS, &UnmatchedOrders::OnCancelOrders, this, wxID_ANY);
}

CurrentOrder* UnmatchedOrders::CreateOrder() {
    return new UnmatchedOrder(this, wxID_ANY);
};

greentop::sport::OrderProjection UnmatchedOrders::GetOrderProjection() const {
    return greentop::sport::OrderProjection::EXECUTABLE;
}

void UnmatchedOrders::OnCancelOrders(wxThreadEvent& event) {
    greentop::sport::CancelExecutionReport cer = event.GetPayload<greentop::sport::CancelExecutionReport>();

    if (cer.isSuccess() && cer.getStatus() == greentop::sport::ExecutionReportStatus::SUCCESS) {
        bool showNoUnmatchedOrdersMessage = true;
        for (unsigned i = 0; i < cer.getInstructionReports().size(); ++i) {
            greentop::sport::CancelInstructionReport cir = cer.getInstructionReports()[i];

            wxWindowList::iterator it = GetChildren().begin();
            while (it != GetChildren().end()) {
                // iterator will be invalidated (?) if the order is erased, so take a copy.
                wxWindowList::const_iterator current = it;
                it++;

                CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*current);
                if (currentOrder) {
                    if (cir.getInstruction().getBetId() == currentOrder->GetCurrentOrderSummary().getBetId()) {
                        currentOrder->Destroy();
                    } else {
                        showNoUnmatchedOrdersMessage = false;
                    }
                }
            }
        }

        noOrdersMessage->Show(showNoUnmatchedOrdersMessage);
    }

    event.Skip();
}

}
}
