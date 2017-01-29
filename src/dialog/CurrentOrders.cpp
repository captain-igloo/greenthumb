/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
 #include <wx/wx.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

#include "dialog/CurrentOrders.h"
#include "worker/ListCurrentOrders.h"
#include "worker/ReplaceOrders.h"
#include "UnmatchedOrder.h"
#include "MatchedOrder.h"

namespace greenthumb {
namespace dialog {

CurrentOrders::CurrentOrders(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name), workerManager(this) {

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    notebook = new wxNotebook(this, wxID_ANY);
    vbox->Add(notebook);

    unmatchedOrders = new wxPanel(notebook, wxID_ANY);
    wxSizer* unmatchedOrdersSizer =  new wxBoxSizer(wxVERTICAL);
    noUnmatchedOrdersMessage = new wxStaticText(unmatchedOrders, wxID_ANY, "There are no unmatched orders.");
    unmatchedOrdersSizer->Add(noUnmatchedOrdersMessage, 1, wxEXPAND);
    notebook->AddPage(unmatchedOrders, "Unmatched");
    unmatchedOrders->SetSizer(unmatchedOrdersSizer);

    matchedOrders = new wxPanel(notebook, wxID_ANY);
    wxSizer* matchedOrdersSizer =  new wxBoxSizer(wxVERTICAL);
    noMatchedOrdersMessage = new wxStaticText(matchedOrders, wxID_ANY, "There are no matched orders.");
    matchedOrdersSizer->Add(noMatchedOrdersMessage, 1, wxEXPAND);
    notebook->AddPage(matchedOrders, "Matched");
    matchedOrders->SetSizer(matchedOrdersSizer);

    wxSizer* buttonSizer = CreateButtonSizer(wxOK);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxALIGN_RIGHT);
    }

    SetSizer(vbox);

    long extraStyle = GetExtraStyle();
    SetExtraStyle(extraStyle - wxWS_EX_BLOCK_EVENTS);

    Bind(worker::LIST_CURRENT_ORDERS, &CurrentOrders::OnListCurrentOrders, this, wxID_ANY);
    workerManager.Bind(worker::LIST_CURRENT_ORDERS);

    Bind(worker::REPLACE_ORDERS, &CurrentOrders::OnReplaceOrders, this, wxID_ANY);
}

void CurrentOrders::OnListCurrentOrders(wxThreadEvent& event) {

    greentop::CurrentOrderSummaryReport currentOrderSummaryReport = event.GetPayload<greentop::CurrentOrderSummaryReport>();

    if (currentOrderSummaryReport.isSuccess()) {
        UpdateOrders(greentop::OrderProjection::EXECUTABLE, unmatchedOrders, currentOrderSummaryReport);
        UpdateOrders(greentop::OrderProjection::EXECUTION_COMPLETE, matchedOrders, currentOrderSummaryReport);
        notebook->InvalidateBestSize();
        unmatchedOrders->GetSizer()->Layout();
        notebook->Fit();
        Fit();
        GetSizer()->Layout();
    }

    event.ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event.Skip();
}

void CurrentOrders::UpdateOrders(const greentop::OrderStatus& orderStatus, wxPanel* ordersPanel,
    const greentop::CurrentOrderSummaryReport& currentOrderSummaryReport) {

    std::set<std::string> betIdsFound;
    std::set<std::string> existingBetIds;

    for (wxWindowList::const_iterator it = ordersPanel->GetChildren().begin(); it != GetChildren().end(); ++it) {
        // CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
        CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
        if (currentOrder) {
            existingBetIds.insert(currentOrder->GetCurrentOrderSummary().getBetId());
        }
    }

    bool showNoUnmatchedOrdersMesssage = true;
    bool showNoMatchedOrdersMesssage = true;

    for (unsigned i = 0; i < currentOrderSummaryReport.getCurrentOrders().size(); ++i) {

        std::string betId = currentOrderSummaryReport.getCurrentOrders()[i].getBetId();

        if (orderStatus == currentOrderSummaryReport.getCurrentOrders()[i].getStatus()) {
            betIdsFound.insert(betId);
            if (orderStatus == greentop::OrderStatus::EXECUTABLE) {
                showNoUnmatchedOrdersMesssage = false;
            } else {
                showNoMatchedOrdersMesssage = false;
            }

            if (existingBetIds.find(betId) == existingBetIds.end()) {
                // new order, add
                wxWindowID currentOrderId = wxWindow::NewControlId();

                CurrentOrder* order;
                if (orderStatus == greentop::OrderStatus::EXECUTABLE) {
                    order = new UnmatchedOrder(ordersPanel, currentOrderId);
                } else {
                    order = new MatchedOrder(ordersPanel, currentOrderId);
                }

                order->SetMarket(market);
                order->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);
                ordersPanel->GetSizer()->Add(order, 0, wxEXPAND);

            } else {
                // already have this one, update it
                wxWindowList::iterator it;
                for (it = ordersPanel->GetChildren().begin(); it != ordersPanel->GetChildren().end(); ++it) {
                    // CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
                    CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
                    if (currentOrder && currentOrder->GetCurrentOrderSummary().getBetId() == betId) {
                        currentOrder->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);
                        break;
                    }
                }
            }
        }
    }

    if (orderStatus == greentop::OrderStatus::EXECUTABLE) {
        noUnmatchedOrdersMessage->Show(showNoUnmatchedOrdersMesssage);
    } else {
        noMatchedOrdersMessage->Show(showNoMatchedOrdersMesssage);
    }

    wxWindowList::iterator it = ordersPanel->GetChildren().begin();
    while (it != ordersPanel->GetChildren().end()) {

        // iterator will be invalidated (?) if the order is erased, so take a copy.
        wxWindowList::const_iterator current = it;
        it++;

        CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*current);
        if (currentOrder) {
            if (betIdsFound.find(currentOrder->GetCurrentOrderSummary().getBetId()) == betIdsFound.end()) {
                currentOrder->Destroy();
            }
        }
    }
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

    for (wxWindowList::const_iterator it = unmatchedOrders->GetChildren().begin(); it != GetChildren().end(); ++it) {
        // CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
        CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
        if (currentOrder) {
            count++;
        }
    }

    for (wxWindowList::const_iterator it = matchedOrders->GetChildren().begin(); it != GetChildren().end(); ++it) {
        // CurrentOrder* currentOrder = wxDynamicCast(*it, CurrentOrder);
        CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
        if (currentOrder) {
            count++;
        }
    }

    return count;
}

}
}
