/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>

#include "worker/ListCurrentOrders.h"
#include "worker/ListMarketCatalogue.h"
#include "CurrentOrdersGrid.h"

namespace greenthumb {

CurrentOrdersGrid::CurrentOrdersGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name) :
    PaginatedGrid(parent, id, pos, size, style, name) {

    grid = new wxGrid(this, wxID_ANY);
    grid->HideRowLabels();
    grid->CreateGrid(0, 8);
    grid->SetColLabelValue(0, "Market");
    grid->SetColumnWidth(0, 150);
    grid->SetColLabelValue(1, "Back/Lay");
    grid->SetColumnWidth(1, 100);
    grid->SetColLabelValue(2, "Selection");
    grid->SetColumnWidth(2, 150);
    grid->SetColLabelValue(3, "Price");
    grid->SetColumnWidth(3, 100);
    grid->SetColFormatFloat(3, -1, 2);
    grid->SetColLabelValue(4, "Stake");
    grid->SetColumnWidth(4, 100);
    grid->SetColFormatFloat(4, -1, 2);
    grid->SetColLabelValue(5, "Av. Price Matched");
    grid->SetColumnWidth(5, 150);
    grid->SetColFormatFloat(5, -1, 2);
    grid->SetColLabelValue(6, "Stake Matched");
    grid->SetColumnWidth(6, 150);
    grid->SetColFormatFloat(6, -1, 2);
    grid->SetColLabelValue(7, "Stake Remaining");
    grid->SetColumnWidth(7, 150);
    grid->SetColFormatFloat(7, -1, 2);

    GetSizer()->Add(grid, 1, wxEXPAND, 0);

    Render();

    Bind(worker::LIST_CURRENT_ORDERS, &CurrentOrdersGrid::OnListCurrentOrders, this, wxID_ANY);
    workerManager.Bind(worker::LIST_CURRENT_ORDERS);
    Bind(worker::LIST_MARKET_CATALOGUE, &CurrentOrdersGrid::OnListMarketCatalogue, this, wxID_ANY);
    workerManager.Bind(worker::LIST_MARKET_CATALOGUE);
}

void CurrentOrdersGrid::SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets) {
    this->betfairMarkets = betfairMarkets;
}

void CurrentOrdersGrid::Refresh() {
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }
    workerManager.RunWorker(new worker::ListCurrentOrders(&workerManager, entity::Market(), currentPage));
}

void CurrentOrdersGrid::OnClickFirst(wxCommandEvent& event) {
    PaginatedGrid::OnClickFirst(event);
    Refresh();
}

void CurrentOrdersGrid::OnClickPrevious(wxCommandEvent& event) {
    PaginatedGrid::OnClickPrevious(event);
    Refresh();
}

void CurrentOrdersGrid::OnClickNext(wxCommandEvent& event) {
    PaginatedGrid::OnClickNext(event);
    Refresh();
}

void CurrentOrdersGrid::OnClickLast(wxCommandEvent& event) {
    PaginatedGrid::OnClickLast(event);
    Refresh();
}

void CurrentOrdersGrid::OnClickRefresh(wxCommandEvent& event) {
    Refresh();
}

void CurrentOrdersGrid::Render() {
    UpdateToolbar();

    grid->BeginBatch();
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }

    if (currentOrderSummaryReport.isSuccess()) {
        for (unsigned i = 0; i < currentOrderSummaryReport.getCurrentOrders().size(); ++i) {
            grid->AppendRows();

            greentop::CurrentOrderSummary cos = currentOrderSummaryReport.getCurrentOrders()[i];
            std::string marketName = cos.getMarketId();
            std::string selection = "";
            if (betfairMarkets->exists(cos.getMarketId())) {
                entity::Market market = betfairMarkets->get(cos.getMarketId());
                marketName = market.GetMarketCatalogue().getEvent().getName() + " / "
                    + market.GetMarketCatalogue().getMarketName();
                if (market.HasRunner(cos.getSelectionId())) {
                    selection = market.GetRunner(cos.getSelectionId()).getRunnerName();
                }
            }

            wxString averagePriceMatched = "";
            if (cos.getAveragePriceMatched().getValue() > 0) {
                averagePriceMatched = wxString::Format(wxT("%f"), cos.getAveragePriceMatched().getValue());
            }
            wxString sizeMatched = "";
            if (cos.getSizeMatched().getValue() > 0) {
                sizeMatched = wxString::Format(wxT("%f"), cos.getSizeMatched().getValue());
            }
            wxString sizeRemaining = "";
            if (cos.getSizeRemaining().getValue() > 0) {
                sizeRemaining = wxString::Format(wxT("%f"), cos.getSizeRemaining().getValue());
            }

            grid->SetCellValue(i, 0, marketName);
            grid->SetCellValue(i, 1, cos.getSide().getValue());
            grid->SetCellValue(i, 2, selection);
            grid->SetCellValue(i, 3, wxString::Format(wxT("%f"), cos.getPriceSize().getPrice().getValue()));
            grid->SetCellValue(i, 4, wxString::Format(wxT("%f"), cos.getPriceSize().getSize().getValue()));
            grid->SetCellValue(i, 5, averagePriceMatched);
            grid->SetCellValue(i, 6, sizeMatched);
            grid->SetCellValue(i, 7, sizeRemaining);
        }

    }

    grid->EndBatch();
}

void CurrentOrdersGrid::OnListMarketCatalogue(const wxThreadEvent& event) {
    std::map<std::string, entity::Market> markets =
        event.GetPayload<std::map<std::string, entity::Market>>();

    for (auto it = markets.begin(); it != markets.end(); ++it) {
        if (it->second.HasMarketCatalogue()) {
            betfairMarkets->put(it->second.GetMarketCatalogue().getMarketId(), it->second);
        }
    }

    Render();
}

void CurrentOrdersGrid::OnListCurrentOrders(const wxThreadEvent& event) {
    currentOrderSummaryReport = event.GetPayload<greentop::CurrentOrderSummaryReport>();

    if (currentOrderSummaryReport.getMoreAvailable() && currentPage == numberPages) {
        // we aren't really on the last page because there are more current orders.
        ++numberPages;
    }

    Render();

    if (currentOrderSummaryReport.isSuccess()) {
        std::set<std::string> marketIds;

        for (unsigned i = 0; i < currentOrderSummaryReport.getCurrentOrders().size(); ++i) {
            greentop::CurrentOrderSummary cos = currentOrderSummaryReport.getCurrentOrders()[i];
            if (!betfairMarkets->exists(cos.getMarketId())) {
                marketIds.insert(cos.getMarketId());
            }
        }

        if (marketIds.size() > 0) {
            workerManager.RunWorker(new worker::ListMarketCatalogue(&workerManager, marketIds));
        }
    }

}

}
