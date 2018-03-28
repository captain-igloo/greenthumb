/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/sizer.h>
#include <greentop/ExchangeApi.h>
#include <greentop/sport/enum/MarketStatus.h>
#include <greentop/sport/enum/RunnerStatus.h>

#include "dialog/PlaceBet.h"
#include "entity/Config.h"
#include "worker/CancelOrders.h"
#include "worker/ListMarketCatalogue.h"
#include "worker/PlaceOrders.h"
#include "worker/ReplaceOrders.h"

#include "MarketPanel.h"
#include "RunnerRow.h"
#include "Util.h"

namespace greenthumb {

MarketPanel::MarketPanel(MarketPanels* parent, const wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name) :
    wxPanel(parent, id, pos, size, style, name), refreshTimer(this, wxID_ANY), workerManager(this) {

    marketPanels = parent;

    wxBoxSizer* tbSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(tbSizer);

    marketToolbar = new MarketToolbar(this, wxID_ANY);
    marketToolbar->Bind(wxEVT_BUTTON, &MarketPanel::OnClickClose, this, marketToolbar->GetCloseButtonId());
    marketToolbar->Bind(wxEVT_BUTTON, &MarketPanel::RefreshPrices, this, marketToolbar->GetRefreshButtonId());
    marketToolbar->Bind(wxEVT_BUTTON, &MarketPanel::ShowCurrentOrders, this, marketToolbar->GetCurrentOrdersButtonId());
    marketToolbar->Bind(wxEVT_BUTTON, &MarketPanel::ShowRules, this, marketToolbar->GetRulesButtonId());
    tbSizer->Add(marketToolbar, 0, wxEXPAND);

    handicapPanel = new market::HandicapPanel(this);
    tbSizer->Add(handicapPanel);

    pricesPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);

    wxFlexGridSizer* sizer = new wxFlexGridSizer(10);
    sizer->AddGrowableCol(1, 1);
    pricesPanel->SetSizer(sizer);

    tbSizer->Add(pricesPanel, 0, wxEXPAND);

    int refreshInterval = entity::Config::GetConfigValue(entity::Config::KEY_REFRESH_INTERVAL, 60);
    refreshTimer.Start(refreshInterval * 1000);

    currentOrdersDialog = new dialog::CurrentOrders(this, wxID_ANY, "Current Bets");

    rulesDialog = new dialog::Html(this, wxID_ANY, "Rules");

    Bind(worker::LIST_MARKET_BOOK, &MarketPanel::OnMarketUpdated, this);
    workerManager.Bind(worker::LIST_MARKET_BOOK);
    Bind(worker::LIST_MARKET_PROFIT_AND_LOSS, &MarketPanel::OnListMarketProfitAndLoss, this);
    workerManager.Bind(worker::LIST_MARKET_PROFIT_AND_LOSS);
    Bind(worker::PLACE_ORDERS, &MarketPanel::OnPlaceBet, this);
    Bind(worker::CANCEL_ORDERS, &MarketPanel::OnCancelOrders, this);
    Bind(wxEVT_TIMER, &MarketPanel::RefreshPrices, this);
    Bind(dialog::PLACE_ORDER_PENDING, &MarketPanel::OnPlaceOrderPending, this);
    Bind(worker::REPLACE_ORDERS, &MarketPanel::RefreshPrices, this, wxID_ANY);
    Bind(market::HANDICAP_CHANGED, &MarketPanel::OnHandicapChanged, this, wxID_ANY);
}

void MarketPanel::OnMarketUpdated(const wxThreadEvent& event) {
    greentop::MarketBook tempMarketBook = event.GetPayload<greentop::MarketBook>();

    if (tempMarketBook.isValid()) {
        marketBook = tempMarketBook;

        market.SetMarketBook(marketBook);

        if (market.GetMarketCatalogue().getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE &&
            !handicapPanel->IsShown()) {
            handicapPanel->Show(true);
            handicapPanel->AddPages(market.GetHandicapPages(), market.GetDefaultHandicapIndex());
        }

        SyncRunnerRows();

        if (marketBook.getStatus() == greentop::MarketStatus::OPEN ||
            marketBook.getStatus() == greentop::MarketStatus::SUSPENDED) {
            refreshTimer.Start();
        } else {
            refreshTimer.Stop();
        }

        UpdateToolBar();
        UpdateMarketStatus();

        worker::ListMarketProfitAndLoss* listMarketProfitAndLossWorker =
            new worker::ListMarketProfitAndLoss(&workerManager, market.GetMarketCatalogue().getMarketId());
        workerManager.RunWorker(listMarketProfitAndLossWorker);
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
    if (market.GetMarketCatalogue().getMarketId() == "") {
        // we don't have the market catalogue, probably means previous attempt to get it failed.
        // try again.
        std::set<std::string> marketIds = { marketId };
        workerManager.RunWorker(new worker::ListMarketCatalogue(&workerManager, marketIds));
    } else {
        worker::ListMarketBook* listMarketBookThread = new worker::ListMarketBook(
            &workerManager,
            market
        );
        workerManager.RunWorker(listMarketBookThread);

        currentOrdersDialog->Refresh();
    }
}

void MarketPanel::RefreshPrices(const wxEvent& event) {
    RefreshPrices();
}

void MarketPanel::SetMarket(const greentop::menu::Node& node) {

    marketId = node.getId();
    std::tm marketStartTime = node.getMarketStartTime();
    time_t startTime = timegm(&marketStartTime);
    char formattedStartTime[20];
    std::strftime(formattedStartTime, 20, "%Y-%m-%d %H:%M:%S", std::localtime(&startTime));

    if (node.hasParent()) {
        fullMarketName = node.getParent().getName() + " / " + node.getName();
        std::string title = fullMarketName + " / " + formattedStartTime;
        wxString label(title.c_str(), wxConvUTF8);
        currentOrdersDialog->SetTitle(fullMarketName);
        marketToolbar->SetMarketName(label);
    }

}

void MarketPanel::SetMarket(const entity::Market& market) {
    this->market = market;
    marketId = market.GetMarketCatalogue().getMarketId();

    SyncRunnerRows();
    RefreshPrices();

    Bind(wxEVT_BUTTON, &MarketPanel::OnClick, this, wxID_ANY);

    currentOrdersDialog->SetMarket(market);
    wxString rules(market.GetMarketCatalogue().getDescription().getRules().c_str(), wxConvUTF8);

    rulesDialog->SetPage(rules);

    UpdateToolBar();
    UpdateMarketStatus();
}

void MarketPanel::UpdateMarketStatus() {
    if (marketBook.getStatus() == greentop::MarketStatus::SUSPENDED ||
        marketBook.getStatus() == greentop::MarketStatus::CLOSED) {
        std::string status = marketBook.getStatus();
        marketToolbar->SetMarketStatus("(" + status + ")");
    } else {
        marketToolbar->SetMarketStatus("");
    }
}

void MarketPanel::UpdateToolBar() {
    if (market.GetMarketCatalogue().getDescription().getTurnInPlayEnabled()) {
        if (marketBook.getInplay().isValid() && marketBook.getInplay().getValue()) {
            marketToolbar->SetInPlay(true);
        } else {
            marketToolbar->SetInPlay(false);
        }
    }
}

void MarketPanel::SyncRunnerRows() {
    std::vector<greentop::Runner> runners = marketBook.getRunners();


    for (unsigned i = 0; i < runners.size(); ++i) {
        greentop::Runner runner = runners[i];

        if (runner.getStatus() == greentop::RunnerStatus::ACTIVE) {
            auto iter = runnerRows.find(runner.getSelectionId());
            if (iter == runnerRows.end()) {
                RunnerRow* runnerRow = new RunnerRow(pricesPanel);
                runnerRows[runner.getSelectionId()] = runnerRow;

                double profit = 0;

                runnerRow->SetRunner(market, marketBook, runner);
                runnerRow->SetProfit(profit);
            } else {
                iter->second->SetRunner(market, marketBook, runner);
            }
        }
    }

    // TODO - remove obsolete runnerrows
    GetParent()->FitInside();
}

void MarketPanel::OnPlaceBet(const wxThreadEvent& event) {
    RefreshPrices();
}

void MarketPanel::OnCancelOrders(const wxThreadEvent& event) {
    RefreshPrices();
}

void MarketPanel::OnClick(const wxCommandEvent& event) {

    PriceButton* button = dynamic_cast<PriceButton*>(event.GetEventObject());

    if (button) {
        greentop::PlaceInstruction pi = button->GetPlaceInstruction();

        if (market.HasRunner(pi.getSelectionId())) {

            auto it = runnerRows.find(pi.getSelectionId());
            if (it != runnerRows.end()) {
                greentop::RunnerCatalog runner = market.GetRunner(pi.getSelectionId());
                wxString placeBetTitle = pi.getSide().getValue() + " " + it->second->GetRunnerName();

                dialog::PlaceBet* placeBet = new dialog::PlaceBet(this, wxID_ANY, placeBetTitle);

                placeBet->SetMarket(market, fullMarketName);
                placeBet->SetPlaceInstruction(runner.getRunnerName(), pi);
                placeBet->Show();
            }
        }
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

void MarketPanel::ShowRules(const wxEvent& event) {
    rulesDialog->Show();
}

void MarketPanel::OnHandicapChanged(const wxEvent& event) {
    if (market.GetMarketCatalogue().getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE) {
        for (const auto &it : runnerRows) {
            RunnerRow* runnerRow = it.second;
            runnerRow->SetHandicap(handicapPanel->GetHandicap(it.first));
        }
    }
}

}
