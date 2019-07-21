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

    // wxSizer* sizer = parent->GetSizer();

    /* chartButtonId = wxWindow::NewControlId();
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
    parent->Bind(wxEVT_BUTTON, &RunnerPrices::OnClickChart, this, chartButtonId); */

    /* runnerName = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    runnerName->SetMinSize(wxSize(150, -1));
    sizer->Add(runnerName, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 10); */

    /* profitAndLossIfWinText = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    profitAndLossIfWinText->SetMinSize(wxSize(150, -1));
    sizer->Add(profitAndLossIfWinText, 0, wxALIGN_CENTRE_VERTICAL);

    pendingProfit = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    pendingProfit->SetMinSize(wxSize(150, -1));
    sizer->Add(pendingProfit, 0, wxALIGN_CENTRE_VERTICAL); */

    /* wxFont font = parent->GetFont();
    font.MakeSmaller();

    bestBackPrice3 = CreateButton(parent, greentop::Side::BACK, wxColour(239, 247, 255));
    bestBackPrice2 = CreateButton(parent, greentop::Side::BACK, wxColour(233, 241, 255));
    bestBackPrice1 = CreateButton(parent, greentop::Side::BACK, wxColour(227, 235, 255));

    bestLayPrice1 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 224, 255));
    bestLayPrice2 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 230, 255));
    bestLayPrice3 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 236, 255)); */

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

/* void RunnerPrices::SetProfit(double handicap, double profitIfWin, double profitIfLose) {
    int64_t scaledHandicap = handicap * scaleFactor;
    profits[scaledHandicap] = std::pair<double, double>(profitIfWin, profitIfLose);
    if (scaledHandicap == this->handicap) {
        // profitAndLossIfWinText->SetLabel("100, 5");
        if (profitIfWin < -0.001 || profitIfWin > 0.001 || profitIfLose < -0.001 || profitIfLose > 0.001) {
            wxString winSign = profitIfWin < -0.001 ? "-" : "";
            wxString loseSign = profitIfLose < -0.001 ? "-" : "";

            wxString profitLabel = winSign + currencySymbol +
                wxString::Format("%.2f", std::abs(profitIfWin)) +
                ", " + loseSign + currencySymbol +
                wxString::Format("%.2f", std::abs(profitIfLose));

            // profitAndLossIfWinText->SetLabel(profitLabel);
        } else {
            // profitAndLossIfWinText->SetLabel("");
        }
        // if (profitAndLossIfWin > 0) {
           //  profitAndLossIfWinText->SetForegroundColour(wxColour(0, 128, 0));
        // } else {
           //  profitAndLossIfWinText->SetForegroundColour(wxColour("RED"));
        // } 
    }
}*/

/*  void RunnerPrices::SetProfit(double handicap, double profit) {
    int64_t scaledHandicap = handicap * scaleFactor;

    profits[scaledHandicap] = std::pair<double, double>(profit, 0);

    if (scaledHandicap == this->handicap) {
        UpdateProfitAndLossIfWin();
    }
} */

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

void RunnerPrices::RefreshPrices() {
    if (GetRunner().getLastPriceTraded().isValid()) {
        lastPriceTraded = GetRunner().getLastPriceTraded().getValue();
    }

    bool marketOpen = marketBook.getStatus() == greentop::MarketStatus::OPEN;

    if (marketOpen && GetRunner().getEx().getAvailableToBack().size() >= 3) {
        SetButtonLabel(
            bestBackPrice3,
            GetRunner().getEx().getAvailableToBack()[2].getPrice(),
            GetRunner().getEx().getAvailableToBack()[2].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestBackPrice3);
    }

    if (marketOpen && GetRunner().getEx().getAvailableToBack().size() >= 2) {
        SetButtonLabel(
            bestBackPrice2,
            GetRunner().getEx().getAvailableToBack()[1].getPrice(),
            GetRunner().getEx().getAvailableToBack()[1].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestBackPrice2);
    }

    if (marketOpen && GetRunner().getEx().getAvailableToBack().size() >= 1) {
        SetButtonLabel(
            bestBackPrice1,
            GetRunner().getEx().getAvailableToBack()[0].getPrice(),
            GetRunner().getEx().getAvailableToBack()[0].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestBackPrice1);
    }

    if (marketOpen && GetRunner().getEx().getAvailableToLay().size() >= 1) {
        SetButtonLabel(
            bestLayPrice1,
            GetRunner().getEx().getAvailableToLay()[0].getPrice(),
            GetRunner().getEx().getAvailableToLay()[0].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestLayPrice1);
    }

    if (marketOpen && GetRunner().getEx().getAvailableToLay().size() >= 2) {
        SetButtonLabel(
            bestLayPrice2,
            GetRunner().getEx().getAvailableToLay()[1].getPrice(),
            GetRunner().getEx().getAvailableToLay()[1].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestLayPrice2);
    }

    if (marketOpen && GetRunner().getEx().getAvailableToLay().size() >= 3) {
        SetButtonLabel(
            bestLayPrice3,
            GetRunner().getEx().getAvailableToLay()[2].getPrice(),
            GetRunner().getEx().getAvailableToLay()[2].getSize(),
            currencySymbol
        );
    } else {
        ResetButton(bestLayPrice3);
    }
}

/* void RunnerPrices::SetPendingPlaceInstruction(const greentop::PlaceInstruction& placeInstruction) {
    double diff = 0;

    if (GetRunner().getSelectionId() == placeInstruction.getSelectionId()) {
        diff = placeInstruction.getLimitOrder().getSize() *
            (placeInstruction.getLimitOrder().getPrice() - 1);
        if (placeInstruction.getSide() == greentop::Side::LAY) {
            // laying this selection
            diff = diff * -1;
        }
    } else {

        diff = placeInstruction.getLimitOrder().getSize();
        if (placeInstruction.getSide() == greentop::Side::BACK) {
            // backing some other selection
            diff = diff * -1;
        }

    }

    if (diff > 0.001 || diff < -0.001) {
        // profitAndLossIfWin
        double pendingProfitAndLoss = profitAndLossIfWin + diff;

        wxString profitLabel = wxString("» ", wxConvUTF8) + currencySymbol
            + wxString::Format("%.2f", std::abs(pendingProfitAndLoss));

        // pendingProfit->SetLabel(profitLabel);

        if (pendingProfitAndLoss > 0) {
            // pendingProfit->SetForegroundColour(wxColour(0, 128, 0));
        } else {
            // pendingProfit->SetForegroundColour(wxColour("RED"));
        }
    } else {
        // pendingProfit->SetLabel("");
    }
} */

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
    wxString runnerName = market.GetRunner(selectionId).getRunnerName();

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
    runnerName->SetMinSize(wxSize(150, -1));
    sizer->Add(runnerName, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 10);
}

void RunnerPrices::CreatePricesButtons(wxWindow* parent) {
    wxFont font = parent->GetFont();
    font.MakeSmaller();
    bestBackPrice3 = CreateButton(parent, greentop::Side::BACK, wxColour(239, 247, 255));
    bestBackPrice2 = CreateButton(parent, greentop::Side::BACK, wxColour(233, 241, 255));
    bestBackPrice1 = CreateButton(parent, greentop::Side::BACK, wxColour(227, 235, 255));

    bestLayPrice1 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 224, 255));
    bestLayPrice2 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 230, 255));
    bestLayPrice3 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 236, 255));
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

/* void RunnerPrices::UpdateProfitAndLossIfWin() {
    auto it = profits.find(handicap);
    if (it != profits.end()) {
        profitAndLossIfWin = it->second.first;

        if (profitAndLossIfWin > 0.001 || profitAndLossIfWin < -0.001) {
            wxString profitLabel = currencySymbol +
                wxString::Format("%.2f", std::abs(profitAndLossIfWin));

            // profitAndLossIfWinText->SetLabel(profitLabel);

            if (profitAndLossIfWin > 0) {
              //  profitAndLossIfWinText->SetForegroundColour(wxColour(0, 128, 0));
            } else {
                // profitAndLossIfWinText->SetForegroundColour(wxColour("RED"));
            }
        } else {
            // profitAndLossIfWinText->SetLabel("");
        }
    } else {
        // profitAndLossIfWinText->SetLabel("");
    }
} */

}
}
}