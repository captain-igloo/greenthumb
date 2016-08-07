/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/timer.h>
#include <greentop/ExchangeApi.h>

#include "dialog/PlaceBet.h"
#include "entity/Config.h"
#include "worker/CancelOrders.h"
#include "worker/ListCurrentOrders.h"
#include "worker/PlaceOrders.h"
#include "worker/ReplaceOrders.h"

#include "ArtProvider.h"
#include "MarketPanel.h"
#include "RunnerRow.h"
#include "Util.h"

namespace greenthumb {

MarketPanel::MarketPanel(MarketPanels* parent, const wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style) : wxPanel(parent, id, pos, size, style),
    refreshTimer(this, wxID_ANY), workerManager(this) {

    marketPanels = parent;

    wxBitmap closeBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::CLOSE);
    wxBitmap refreshBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::REFRESH);
    wxBitmap inPlayBlank = ArtProvider::GetBitmap(ArtProvider::IconId::BLANK);
    wxBitmap currentOrdersBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::VIEW_LIST);

    toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 50));

    wxToolBarToolBase* inPlayTool = toolbar->AddTool(wxID_ANY, _(""), inPlayBlank);
    inPlayToolId = inPlayTool->GetId();

    marketName = new wxStaticText(toolbar, wxID_ANY, _(""));
    toolbar->AddControl(marketName);

    marketStatus = new wxStaticText(toolbar, wxID_ANY, _(""));
    marketStatus->SetForegroundColour(wxColour(255, 0, 0));
    toolbar->AddControl(marketStatus);

    toolbar->AddStretchableSpace();

    wxWindowID currentOrdersButtonId = wxWindow::NewControlId();
    currentOrdersTool = toolbar->AddTool(currentOrdersButtonId, "", currentOrdersBitmap, "Show current bets");
    toolbar->EnableTool(currentOrdersTool->GetId(), false);
    Bind(wxEVT_TOOL, &MarketPanel::ShowCurrentOrders, this, currentOrdersButtonId);

    wxWindowID refreshButtonId = wxWindow::NewControlId();
    toolbar->AddTool(refreshButtonId, "", refreshBitmap, "Refresh");
    Bind(wxEVT_TOOL, &MarketPanel::RefreshPrices, this, refreshButtonId);

    wxWindowID closeButtonId = wxWindow::NewControlId();
    toolbar->AddTool(closeButtonId, "", closeBitmap, "Close");
    Bind(wxEVT_TOOL, &MarketPanel::OnClickClose, this, closeButtonId);

    toolbar->Realize();

    wxBoxSizer* tbSizer = new wxBoxSizer(wxVERTICAL);
    tbSizer->Add(toolbar, 0, wxEXPAND);
    SetSizer(tbSizer);

    leftPanel = new wxPanel(this);

    wxFlexGridSizer* sizer = new wxFlexGridSizer(10);
    sizer->AddGrowableCol(1, 1);
    leftPanel->SetSizer(sizer);

    tbSizer->Add(leftPanel);

    refreshTimer.Start(entity::Config::GetConfigValue("marketRefreshSec", 60) * 1000);

    currentOrdersDialog = new dialog::CurrentOrders(this, wxID_ANY, "Current Bets");

    Bind(worker::LIST_MARKET_BOOK, &MarketPanel::OnMarketUpdated, this);
    workerManager.Bind(worker::LIST_MARKET_BOOK);
    Bind(worker::LIST_MARKET_PROFIT_AND_LOSS, &MarketPanel::OnListMarketProfitAndLoss, this);
    workerManager.Bind(worker::LIST_MARKET_PROFIT_AND_LOSS);
    Bind(worker::PLACE_ORDERS, &MarketPanel::OnPlaceBet, this);
    Bind(worker::CANCEL_ORDERS, &MarketPanel::OnCancelOrders, this);
    Bind(wxEVT_TIMER, &MarketPanel::RefreshPrices, this);
    Bind(dialog::PLACE_ORDER_PENDING, &MarketPanel::OnPlaceOrderPending, this);
    Bind(worker::LIST_CURRENT_ORDERS, &MarketPanel::OnListCurrentOrders, this, wxID_ANY);
    Bind(worker::REPLACE_ORDERS, &MarketPanel::RefreshPrices, this, wxID_ANY);
}

void MarketPanel::OnMarketUpdated(const wxThreadEvent& event) {

    greentop::MarketBook tempMarketBook = event.GetPayload<greentop::MarketBook>();

    if (tempMarketBook.isValid()) {
        marketBook = tempMarketBook;

        SyncRunnerRows();

        currentOrdersDialog->Refresh();

        if (marketBook.getStatus() == greentop::MarketStatus::OPEN ||
            marketBook.getStatus() == greentop::MarketStatus::SUSPENDED) {
            refreshTimer.Start();
        } else {
            refreshTimer.Stop();
        }

        worker::ListMarketProfitAndLoss* listMarketProfitAndLossWorker =
            new worker::ListMarketProfitAndLoss(&workerManager, market.GetExchange(),
                market.GetMarketCatalogue().getMarketId());
        workerManager.RunWorker(listMarketProfitAndLossWorker);

        UpdateToolBar();
        UpdateMarketStatus();
    }
}

void MarketPanel::OnListMarketProfitAndLoss(const wxThreadEvent& event) {
    greentop::ListMarketProfitAndLossResponse lmpalr = event.GetPayload<greentop::ListMarketProfitAndLossResponse>();

    if (lmpalr.isSuccess()) {
        std::vector<greentop::MarketProfitAndLoss> marketProfitAndLosses = lmpalr.getMarketProfitAndLosses();

        std::vector<greentop::MarketProfitAndLoss>::iterator it1;
        for (it1 = marketProfitAndLosses.begin(); it1 != marketProfitAndLosses.end(); ++it1) {

            greentop::MarketProfitAndLoss marketProfitAndLoss = *it1;

            if (marketProfitAndLoss.getMarketId() == market.GetMarketCatalogue().getMarketId()) {

                std::vector<greentop::RunnerProfitAndLoss> profitAndLosses = marketProfitAndLoss.getProfitAndLosses();

                std::vector<greentop::RunnerProfitAndLoss>::iterator it2;
                for (it2 = profitAndLosses.begin(); it2 != profitAndLosses.end(); ++it2) {
                    greentop::RunnerProfitAndLoss runnerProfitAndLoss = *it2;
                    if (runnerRows.find(runnerProfitAndLoss.getSelectionId()) != runnerRows.end()) {
                        greentop::Optional<double> optionalIfWin = runnerProfitAndLoss.getIfWin();
                        double ifWin = 0;
                        if (optionalIfWin.isValid()) {
                            ifWin = optionalIfWin.getValue();
                        }
                        runnerRows[runnerProfitAndLoss.getSelectionId()]->SetProfit(ifWin);
                    }
                }
            }
        }

        GetParent()->Layout();
    }
}

void MarketPanel::RefreshPrices() {
    worker::ListMarketBook* listMarketBookThread = new worker::ListMarketBook(&workerManager, market);
    workerManager.RunWorker(listMarketBookThread);
}

void MarketPanel::RefreshPrices(wxEvent& event) {
    RefreshPrices();
}

void MarketPanel::SetMarket(const greentop::menu::Node& node) {

    marketId = node.getId();
    // FIXME not portable ?
    std::tm marketStartTime = node.getMarketStartTime();
    time_t startTime = timegm(&marketStartTime);
    char formattedStartTime[20];
    std::strftime(formattedStartTime, 20, "%F %T", std::localtime(&startTime));

    if (node.hasParent()) {
        fullMarketName = node.getParent().getName() + " / " + node.getName();
        std::string title = fullMarketName + " / " + formattedStartTime;
        wxString label(title.c_str(), wxConvUTF8);
        marketName->SetLabel(label);
        currentOrdersDialog->SetTitle(fullMarketName);
    }

}

void MarketPanel::SetMarket(const entity::Market& market) {

    this->market = market;
    marketId = market.GetMarketCatalogue().getMarketId();

    SyncRunnerRows();
    RefreshPrices();

    Bind(wxEVT_BUTTON, &MarketPanel::OnClick, this, wxID_ANY);

    currentOrdersDialog->SetMarket(market);
    // currentOrdersDialog->SetRunners(runners);

    UpdateToolBar();
    UpdateMarketStatus();

}

void MarketPanel::UpdateMarketStatus() {
    if (marketBook.getStatus() == greentop::MarketStatus::SUSPENDED ||
        marketBook.getStatus() == greentop::MarketStatus::CLOSED) {
        std::string status = marketBook.getStatus();
        marketStatus->SetLabel("(" + status + ")");
    } else {
        marketStatus->SetLabel("");
    }
}

void MarketPanel::UpdateToolBar() {
    if (market.GetMarketCatalogue().getDescription().getTurnInPlayEnabled()) {
        if (marketBook.getInplay().isValid() && marketBook.getInplay().getValue()) {
            toolbar->DeleteTool(inPlayToolId);
            wxBitmap inPlayGreenBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::TICK_GREEN);
            wxToolBarToolBase* inPlayTool = toolbar->InsertTool(0, wxID_ANY, wxT(""), inPlayGreenBitmap);
            inPlayToolId = inPlayTool->GetId();
        } else {
            toolbar->DeleteTool(inPlayToolId);
            wxBitmap inPlayGreyBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::TICK_GREY);
            wxToolBarToolBase* inPlayTool = toolbar->InsertTool(0, wxID_ANY, wxT(""), inPlayGreyBitmap);
            inPlayToolId = inPlayTool->GetId();

        }
        toolbar->Realize();
    }
}

void MarketPanel::SyncRunnerRows() {

    std::vector<greentop::Runner> runners = marketBook.getRunners();

    for (unsigned i = 0; i < runners.size(); ++i) {
        greentop::Runner runner = runners[i];

        auto iter = runnerRows.find(runner.getSelectionId());
        if (iter == runnerRows.end()) {
            RunnerRow* runnerRow = new RunnerRow(leftPanel);
            runnerRows[runner.getSelectionId()] = runnerRow;

            double profit = 0;

            runnerRow->SetRunner(market, marketBook, runner);
            runnerRow->SetProfit(profit);
        } else {
            iter->second->SetRunner(market, marketBook, runner);
        }
    }

    // TODO - remove obsolete runnerrows
    GetParent()->Layout();

}

void MarketPanel::OnPlaceBet(const wxThreadEvent& event) {
    RefreshPrices();
}

void MarketPanel::OnCancelOrders(const wxThreadEvent& event) {
    RefreshPrices();
}

void MarketPanel::OnClick(const wxCommandEvent& event) {

    PriceButton* button = static_cast<PriceButton*>(event.GetEventObject());

    greentop::PlaceInstruction pi = button->GetPlaceInstruction();

    if (market.HasRunner(pi.getSelectionId())) {
        greentop::RunnerCatalog runner = market.GetRunner(pi.getSelectionId());
        std::string placeBetTitle = pi.getSide().getValue() + " " + runner.getRunnerName();

        dialog::PlaceBet* placeBet = new dialog::PlaceBet(this, wxID_ANY, placeBetTitle);

        placeBet->SetMarket(market, fullMarketName);
        placeBet->SetPlaceInstruction(runner.getRunnerName(), pi);
        placeBet->Show();
    }

}

void MarketPanel::OnClickClose(const wxCommandEvent& event) {
    marketPanels->RemoveMarket(marketId);
}

void MarketPanel::OnPlaceOrderPending(const wxCommandEvent& event) {

    greentop::PlaceInstruction* pi = static_cast<greentop::PlaceInstruction*>(event.GetClientData());

    for (const auto &it : runnerRows) {
        RunnerRow* runnerRow = it.second;
        runnerRow->SetPendingPlaceInstruction(*pi);
    }

    GetParent()->Layout();
}

void MarketPanel::ShowCurrentOrders(const wxEvent& event) {
    currentOrdersDialog->Show();
}

void MarketPanel::OnListCurrentOrders(wxThreadEvent& event) {

    bool enabled = false;

    if (currentOrdersDialog->GetNumberOrders() > 0) {
        enabled = true;
    }

    toolbar->EnableTool(currentOrdersTool->GetId(), enabled);
}

}
