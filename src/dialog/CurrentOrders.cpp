/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "dialog/CurrentOrders.h"
#include "worker/ListCurrentOrders.h"
#include "worker/ReplaceOrders.h"

#include "CurrentOrder.h"

namespace greenthumb {
namespace dialog {

CurrentOrders::CurrentOrders(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name), workerManager(this) {

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    long extraStyle = GetExtraStyle();
    SetExtraStyle(extraStyle - wxWS_EX_BLOCK_EVENTS);

    Bind(worker::LIST_CURRENT_ORDERS, &CurrentOrders::OnListCurrentOrders, this, wxID_ANY);
    workerManager.Bind(worker::LIST_CURRENT_ORDERS);

    Bind(worker::REPLACE_ORDERS, &CurrentOrders::OnReplaceOrders, this, wxID_ANY);
}

void CurrentOrders::OnListCurrentOrders(wxThreadEvent& event) {

    greentop::CurrentOrderSummaryReport currentOrderSummaryReport = event.GetPayload<greentop::CurrentOrderSummaryReport>();

    if (currentOrderSummaryReport.isSuccess()) {

        std::set<std::string> betIdsFound;
        std::set<std::string> existingBetIds;

        for (wxWindowList::const_iterator it = GetChildren().begin(); it != GetChildren().end(); ++it) {
            CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
            if (currentOrder) {
                existingBetIds.insert(currentOrder->GetCurrentOrderSummary().getBetId());
            }
        }

        for (unsigned i = 0; i < currentOrderSummaryReport.getCurrentOrders().size(); ++i) {

            std::string betId = currentOrderSummaryReport.getCurrentOrders()[i].getBetId();
            betIdsFound.insert(betId);

            if (existingBetIds.find(betId) == existingBetIds.end()) {
                // new order, add
                wxWindowID currentOrderId = wxWindow::NewControlId();

                CurrentOrder* co = new CurrentOrder(this, currentOrderId);
                co->SetMarket(market);
                co->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);

                GetSizer()->Add(co, 1, wxEXPAND);

            } else {
                // already have this one, update it
                wxWindowList::iterator it;
                for (it = GetChildren().begin(); it != GetChildren().end(); ++it) {
                    CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
                    if (currentOrder && currentOrder->GetCurrentOrderSummary().getBetId() == betId) {
                        currentOrder->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);
                        break;
                    }
                }
            }

        }

        wxWindowList::iterator it = GetChildren().begin();
        while (it != GetChildren().end()) {

            // iterator will be invalidated (?) if the order is erased, so take a copy.
            wxWindowList::const_iterator current = it;
            it++;

            CurrentOrder* currentOrder = wxDynamicCast(*current, CurrentOrder);
            if (currentOrder) {
                if (betIdsFound.find(currentOrder->GetCurrentOrderSummary().getBetId()) == betIdsFound.end()) {
                    currentOrder->Destroy();
                }
            }

        }

        GetSizer()->Fit(this);
        Layout();

    }

    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

void CurrentOrders::SetMarket(const entity::Market& market) {
    this->market = market;
}

void CurrentOrders::Refresh() {
    workerManager.RunWorker(new worker::ListCurrentOrders(&workerManager, market));
}

void CurrentOrders::OnReplaceOrders(wxThreadEvent& event) {
    Refresh();
    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

uint64_t CurrentOrders::GetNumberOrders() {

    uint64_t count = 0;

    for (wxWindowList::const_iterator it = GetChildren().begin(); it != GetChildren().end(); ++it) {
        CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
        if (currentOrder) {
            count++;
        }
    }

    return count;
}

}
}
