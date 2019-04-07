/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
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
    Bind(worker::REPLACE_ORDERS, &UnmatchedOrders::OnReplaceOrders, this, wxID_ANY);    
}

CurrentOrder* UnmatchedOrders::CreateOrder() {
    return new UnmatchedOrder(this, wxID_ANY);
};

greentop::OrderProjection UnmatchedOrders::GetOrderProjection() const {
    return greentop::OrderProjection::EXECUTABLE;
}

void UnmatchedOrders::OnCancelOrders(wxThreadEvent& event) {
    greentop::CancelExecutionReport cer = event.GetPayload<greentop::CancelExecutionReport>();

    if (cer.isSuccess() && cer.getStatus() == greentop::ExecutionReportStatus::SUCCESS) {
        bool showNoUnmatchedOrdersMessage = true;
        for (unsigned i = 0; i < cer.getInstructionReports().size(); ++i) {
            greentop::CancelInstructionReport cir = cer.getInstructionReports()[i];

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
        RefreshOrders();
    }

    event.Skip();
}

void UnmatchedOrders::OnReplaceOrders(wxThreadEvent& event) {
    RefreshOrders();
}

}
}
