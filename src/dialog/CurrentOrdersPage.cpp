/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */

#include "dialog/CurrentOrdersPage.h"
#include "worker/ListCurrentOrders.h"

namespace greenthumb {
namespace dialog {

CurrentOrdersPage::CurrentOrdersPage(wxWindow* parent) : wxPanel(parent), workerManager(this) {
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    toolbar = new PaginationToolbar(this, wxID_ANY);
    sizer->Add(toolbar, 0, wxEXPAND);
    noOrdersMessage = new wxStaticText(this, wxID_ANY, "");
    sizer->Add(noOrdersMessage, 1, wxTOP | wxBOTTOM | wxLEFT | wxEXPAND, 10);
    SetSizer(sizer);

    toolbar->Bind(wxEVT_TOOL, &CurrentOrdersPage::OnClickRefresh, this, toolbar->GetRefreshId());
    toolbar->Bind(PAGE_CHANGED, &CurrentOrdersPage::OnPageChanged, this, wxID_ANY);
    workerManager.Bind(worker::LIST_CURRENT_ORDERS);
    Bind(worker::LIST_CURRENT_ORDERS, &CurrentOrdersPage::OnListCurrentOrders, this, wxID_ANY);
}

void CurrentOrdersPage::SetMarket(const entity::Market& market) {
    this->market = market;
    RefreshOrders();
}

void CurrentOrdersPage::OnClickRefresh(const wxCommandEvent& event) {
    RefreshOrders();
}

void CurrentOrdersPage::OnPageChanged(const wxCommandEvent& event) {
    RefreshOrders();
}

void CurrentOrdersPage::RefreshOrders() {
    if (market.HasMarketCatalogue()) {
        workerManager.RunWorker(
            new worker::ListCurrentOrders(
                &workerManager,
                market,
                GetOrderProjection(),
                toolbar->GetCurrentPage()
            )
        );
    }
}

void CurrentOrdersPage::OnListCurrentOrders(const wxThreadEvent& event) {
    greentop::CurrentOrderSummaryReport currentOrderSummaryReport = event.GetPayload<greentop::CurrentOrderSummaryReport>();
    if (currentOrderSummaryReport.isSuccess()) {
        std::set<std::string> betIdsFound;
        std::set<std::string> existingBetIds;

        for (wxWindowList::const_iterator it = GetChildren().begin(); it != GetChildren().end(); ++it) {
            CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
            if (currentOrder) {
                existingBetIds.insert(currentOrder->GetCurrentOrderSummary().getBetId());
            }
        }
        bool showNoOrdersMesssage = true;

        for (unsigned i = 0; i < currentOrderSummaryReport.getCurrentOrders().size(); ++i) {
            std::string betId = currentOrderSummaryReport.getCurrentOrders()[i].getBetId();

            betIdsFound.insert(betId);
            showNoOrdersMesssage = false;

            if (existingBetIds.find(betId) == existingBetIds.end()) {
                // new order, add
                wxWindowID currentOrderId = wxWindow::NewControlId();
                CurrentOrder* order = CreateOrder();
                order->SetMarket(market);
                order->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);
                GetSizer()->Add(order, 0, wxEXPAND);

            } else {
                // already have this one, update it
                wxWindowList::iterator it;
                for (it = GetChildren().begin(); it != GetChildren().end(); ++it) {
                    CurrentOrder* currentOrder = dynamic_cast<CurrentOrder*>(*it);
                    if (currentOrder && currentOrder->GetCurrentOrderSummary().getBetId() == betId) {
                        currentOrder->SetCurrentOrderSummary(currentOrderSummaryReport.getCurrentOrders()[i]);
                        break;
                    }
                }
            }
        }
        noOrdersMessage->Show(showNoOrdersMesssage);

        wxWindowList::iterator it = GetChildren().begin();
        while (it != GetChildren().end()) {
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

        bool moreAvailable = currentOrderSummaryReport.getMoreAvailable();
        toolbar->SetMoreAvailable(currentOrderSummaryReport.getMoreAvailable());

        Layout();
        GetParent()->Layout();
        GetParent()->GetParent()->Layout();
        GetParent()->GetParent()->Fit();
    }
}

}
}
