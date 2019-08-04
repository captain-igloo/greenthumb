/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#include <greentop/sport/enum/MarketStatus.h>
#include <greentop/sport/RunnerCatalog.h>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>

#include <wx/wx.h>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "dialog/PriceHistory.h"
#include "entity/Config.h"
#include "widget/market/MultiWinnerRunnerPrices.h"
#include "widget/market/RunnerPrices.h"
#include "widget/market/SingleWinnerRunnerPrices.h"
#include "ArtProvider.h"
#include "GreenThumb.h"
#include "Util.h"

namespace greenthumb {
namespace widget {
namespace market {

const wxColour RunnerPrices::backColour1 = wxColour(227, 235, 255);
const wxColour RunnerPrices::backColour2 = wxColour(233, 241, 255);
const wxColour RunnerPrices::backColour3 = wxColour(239, 247, 255);

const wxColour RunnerPrices::layColour1 = wxColour(255, 224, 255);
const wxColour RunnerPrices::layColour2 = wxColour(255, 230, 255);
const wxColour RunnerPrices::layColour3 = wxColour(255, 236, 255);

RunnerPrices* RunnerPrices::GetInstance(wxWindow* parent, bool multiWinner) {
    if (multiWinner) {
        return new MultiWinnerRunnerPrices(parent);
    }
    return new SingleWinnerRunnerPrices(parent);
}

RunnerPrices::RunnerPrices(wxWindow* parent) : lastPriceTraded(-1) {
    currencySymbol = GetCurrencySymbol(
        entity::Config::GetConfigValue<wxString>("accountCurrency", "?")
    );
}

PriceButton* RunnerPrices::CreateButton(
    wxWindow* parent,
    const greentop::Side& side,
    const wxColour& colour
) const {
    PriceButton* button = new PriceButton(parent);
    button->SetBackgroundColour(colour);
    button->SetSide(side);
    button->SetHandicap(static_cast<double>(handicap) / scaleFactor);

    wxFont font = parent->GetFont();
    font.MakeSmaller();
    button->SetFont(font);

    parent->GetSizer()->Add(button);

    return button;
}

const int64_t RunnerPrices::GetSelectionId() const {
    return selectionId;
}

void RunnerPrices::SetRunner(
    const entity::Market& market,
    const greentop::MarketBook& marketBook,
    const greentop::Runner& runner
) {
    selectionId = runner.getSelectionId();

    int64_t runnerHandicap = 0;
    if (runner.getHandicap().isValid()) {
        runnerHandicap = runner.getHandicap().getValue() * scaleFactor;
    }
    runners[runnerHandicap] = runner;
    if (runners.find(handicap) == runners.end()) {
        handicap = runnerHandicap;
    }

    if (market.GetMarketCatalogue().getDescription().getBettingType() ==
            greentop::MarketBettingType::ASIAN_HANDICAP_SINGLE_LINE) {
        handicap = runnerHandicap;
    }

    this->market = market;
    this->marketBook = marketBook;

    bestBackPrice3->SetSelectionId(runner.getSelectionId());
    bestBackPrice2->SetSelectionId(runner.getSelectionId());
    bestBackPrice1->SetSelectionId(runner.getSelectionId());
    bestLayPrice1->SetSelectionId(runner.getSelectionId());
    bestLayPrice2->SetSelectionId(runner.getSelectionId());
    bestLayPrice3->SetSelectionId(runner.getSelectionId());

    if (handicap == runnerHandicap) {
        RefreshPrices();
    }
    UpdateRunnerName();
}

void RunnerPrices::RefreshPrice(
    bool marketOpen,
    const greentop::Side& side,
    unsigned index,
    PriceButton* priceButton
) {
    std::vector<greentop::PriceSize> prices;
    if (side == greentop::Side::BACK) {
        prices = GetRunner().getEx().getAvailableToBack();
    } else {
        prices = GetRunner().getEx().getAvailableToLay();
    }

    if (marketOpen && prices.size() >= (index + 1)) {
        SetButtonLabel(
            priceButton,
            prices[index].getPrice(),
            prices[index].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(priceButton);
    }
}

void RunnerPrices::RefreshPrices() {
    if (GetRunner().getLastPriceTraded().isValid()) {
        lastPriceTraded = GetRunner().getLastPriceTraded().getValue();
    } else {
        lastPriceTraded = -1;
    }

    bool marketOpen = marketBook.getStatus() == greentop::MarketStatus::OPEN;

    RefreshPrice(marketOpen, greentop::Side::BACK, 2, bestBackPrice3);
    RefreshPrice(marketOpen, greentop::Side::BACK, 1, bestBackPrice2);
    RefreshPrice(marketOpen, greentop::Side::BACK, 0, bestBackPrice1);
    RefreshPrice(marketOpen, greentop::Side::LAY, 0, bestLayPrice1);
    RefreshPrice(marketOpen, greentop::Side::LAY, 1, bestLayPrice2);
    RefreshPrice(marketOpen, greentop::Side::LAY, 2, bestLayPrice3);
}

void RunnerPrices::SetButtonLabel(
    PriceButton* button,
    const double price,
    const double size,
    const wxString& currency
) const {
    wxString label = wxString::Format(wxT("%.2f\n"), price) + currency +
        wxString::Format(wxT("%.0f"), size);
    button->SetLabel(label);
    button->SetPrice(price);
    button->SetHandicap(static_cast<double>(handicap) / scaleFactor);
}

void RunnerPrices::ResetButton(PriceButton* button) const {
    button->SetLabel("");

    double price;
    if (button->GetSide() == greentop::Side::BACK) {
        price = 100;
    } else {
        price = 1.01;
    }
    button->SetPrice(price);
    button->SetHandicap(static_cast<double>(handicap) / scaleFactor);
}

void RunnerPrices::OnClickChart(wxEvent& event) {
    dialog::PriceHistory priceHistory(NULL, wxID_ANY, "Price History");
    priceHistory.SetLastPriceTraded(lastPriceTraded);
    priceHistory.SetRunner(market, GetRunner());
    priceHistory.ShowModal();
}

void RunnerPrices::SetHandicap(const double handicap) {
    int64_t scaledHandicap = handicap * scaleFactor;
    if ((runners.find(scaledHandicap) != runners.end()) && market.HasRunner(selectionId)) {
        this->handicap = scaledHandicap;
        RefreshPrices();
        UpdateRunnerName();
        UpdateProfitAndLossIfWin();
    }
}

const greentop::Runner& RunnerPrices::GetRunner() {
    return runners[handicap];
}

const wxString RunnerPrices::GetRunnerName() const {
    wxString runnerName(market.GetRunner(selectionId).getRunnerName().c_str(), wxConvUTF8);

    if (handicap != 0 && (runners.find(handicap) != runners.end()) && market.HasRunner(selectionId)) {
        runnerName = GetSelectionName(
            market.GetMarketCatalogue(),
            market.GetRunner(selectionId),
            runners.find(handicap)->second.getHandicap()
        );
    }
    return runnerName;
}

void RunnerPrices::CreateChartButton(wxWindow* parent) {
    wxSizer* sizer = parent->GetSizer();
    chartButtonId = wxWindow::NewControlId();
    wxBitmap chartBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::CHART);
    wxButton* chartButton = new wxButton(
        parent,
        chartButtonId,
        wxEmptyString,
        wxDefaultPosition,
        wxSize(chartButtonWidth, -1),
        wxBORDER_NONE
    );
    chartButton->SetBitmap(chartBitmap);
    sizer->Add(chartButton, 0, wxALIGN_CENTRE_VERTICAL);
    parent->Bind(wxEVT_BUTTON, &RunnerPrices::OnClickChart, this, chartButtonId);
}

void RunnerPrices::CreateRunnerName(wxWindow* parent) {
    wxSizer* sizer = parent->GetSizer();
    runnerName = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    runnerName->SetMinSize(wxSize(250, -1));
    sizer->Add(runnerName, 1, wxALIGN_CENTRE_VERTICAL | wxLEFT, 10);
}

void RunnerPrices::CreatePricesButtons(wxWindow* parent) {
    bestBackPrice3 = CreateButton(parent, greentop::Side::BACK, RunnerPrices::backColour3);
    bestBackPrice2 = CreateButton(parent, greentop::Side::BACK, RunnerPrices::backColour2);
    bestBackPrice1 = CreateButton(parent, greentop::Side::BACK, RunnerPrices::backColour1);

    bestLayPrice1 = CreateButton(parent, greentop::Side::LAY, RunnerPrices::layColour1);
    bestLayPrice2 = CreateButton(parent, greentop::Side::LAY, RunnerPrices::layColour2);
    bestLayPrice3 = CreateButton(parent, greentop::Side::LAY, RunnerPrices::layColour3);
}

void RunnerPrices::UpdateRunnerName() {
    runnerName->SetLabel(GetRunnerName());
}

void RunnerPrices::SetProfit(double handicap, double profitIfWin, double profitIfLose) {
    int64_t scaledHandicap = handicap * scaleFactor;

    profits[scaledHandicap] = std::pair<double, double>(profitIfWin, profitIfLose);

    if (scaledHandicap == this->handicap) {
        UpdateProfitAndLossIfWin();
    }
}

}
}
}