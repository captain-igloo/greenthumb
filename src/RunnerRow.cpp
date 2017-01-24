/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <greentop/sport/enum/MarketStatus.h>
#include <greentop/sport/RunnerCatalog.h>

#include "dialog/PriceHistory.h"
#include "entity/Config.h"
#include "ArtProvider.h"
#include "GreenThumb.h"
#include "RunnerRow.h"
#include "Util.h"

namespace greenthumb {

RunnerRow::RunnerRow(wxWindow* parent) : lastPriceTraded(-1), profitAndLossIfWin(0) {

    currencySymbol = GetCurrencySymbol(entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));

    wxSizer* sizer = parent->GetSizer();

    chartButtonId = wxWindow::NewControlId();
    wxBitmap chartBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::CHART);
    wxButton* chartButton = new wxButton(parent, chartButtonId, wxEmptyString, wxDefaultPosition,
        wxSize(chartButtonWidth, -1), wxBORDER_NONE);
    chartButton->SetBitmap(chartBitmap);
    sizer->Add(chartButton, 0, wxALIGN_CENTRE_VERTICAL);
    parent->Bind(wxEVT_BUTTON, &RunnerRow::OnClickChart, this, chartButtonId);

    runnerName = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    runnerName->SetMinSize(wxSize(150, -1));
    sizer->Add(runnerName, 0, wxALIGN_CENTRE_VERTICAL | wxLEFT, 10);

    profitAndLossIfWinText = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    profitAndLossIfWinText->SetMinSize(wxSize(75, -1));
    sizer->Add(profitAndLossIfWinText, 0, wxALIGN_CENTRE_VERTICAL);

    pendingProfit = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    pendingProfit->SetMinSize(wxSize(75, -1));
    sizer->Add(pendingProfit, 0, wxALIGN_CENTRE_VERTICAL);

    wxFont font = parent->GetFont();
    font.MakeSmaller();

    bestBackPrice3 = CreateButton(parent, greentop::Side::BACK, wxColour(239, 247, 255));
    bestBackPrice2 = CreateButton(parent, greentop::Side::BACK, wxColour(233, 241, 255));
    bestBackPrice1 = CreateButton(parent, greentop::Side::BACK, wxColour(227, 235, 255));

    bestLayPrice1 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 224, 255));
    bestLayPrice2 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 230, 255));
    bestLayPrice3 = CreateButton(parent, greentop::Side::LAY, wxColour(255, 236, 255));

}

PriceButton* RunnerRow::CreateButton(wxWindow* parent, const greentop::Side& side, const wxColour& colour) const {

    PriceButton* button = new PriceButton(parent);
    button->SetBackgroundColour(colour);
    button->SetSide(side);

    wxFont font = parent->GetFont();
    font.MakeSmaller();
    button->SetFont(font);

    parent->GetSizer()->Add(button);

    return button;
}

uint64_t RunnerRow::GetSelectionId() const {
    return runner.getSelectionId();
}

void RunnerRow::SetProfit(double profit) {

    profitAndLossIfWin = profit;

    if (profitAndLossIfWin > 0.001 || profitAndLossIfWin < -0.001) {
        std::ostringstream profitLabelStream;
        profitLabelStream << std::fixed << std::setprecision(2) << std::abs(profitAndLossIfWin);
        wxString profitLabel((currencySymbol + profitLabelStream.str()).c_str(), wxConvUTF8);
        profitAndLossIfWinText->SetLabel(profitLabel);

        if (profitAndLossIfWin > 0) {
            profitAndLossIfWinText->SetForegroundColour(wxColour(0, 128, 0));
        } else {
            profitAndLossIfWinText->SetForegroundColour(wxColour("RED"));
        }
    } else {
        profitAndLossIfWinText->SetLabel("");
    }

}

void RunnerRow::SetRunner(const entity::Market& market, const greentop::MarketBook& marketBook, const greentop::Runner& runner) {

    this->market = market;
    this->marketBook = marketBook;
    this->runner = runner;

    if (market.HasRunner(runner.getSelectionId())) {
        wxString label(market.GetRunner(runner.getSelectionId()).getRunnerName());
        runnerName->SetLabel(label);
    }

    bestBackPrice3->SetSelectionId(runner.getSelectionId());
    bestBackPrice2->SetSelectionId(runner.getSelectionId());
    bestBackPrice1->SetSelectionId(runner.getSelectionId());
    bestLayPrice1->SetSelectionId(runner.getSelectionId());
    bestLayPrice2->SetSelectionId(runner.getSelectionId());
    bestLayPrice3->SetSelectionId(runner.getSelectionId());

    RefreshPrices();

}

void RunnerRow::RefreshPrices() {

    if (runner.getLastPriceTraded().isValid()) {
        lastPriceTraded = runner.getLastPriceTraded().getValue();
    }

    bool marketOpen = marketBook.getStatus() == greentop::MarketStatus::OPEN;

    wxString wxCurrencySymbol(currencySymbol.c_str(), wxConvUTF8);

    if (marketOpen && runner.getEx().getAvailableToBack().size() >= 3) {
        SetButtonLabel(
            bestBackPrice3,
            runner.getEx().getAvailableToBack()[2].getPrice(),
            runner.getEx().getAvailableToBack()[2].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestBackPrice3);
    }

    if (marketOpen && runner.getEx().getAvailableToBack().size() >= 2) {
        SetButtonLabel(
            bestBackPrice2,
            runner.getEx().getAvailableToBack()[1].getPrice(),
            runner.getEx().getAvailableToBack()[1].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestBackPrice2);
    }

    if (marketOpen && runner.getEx().getAvailableToBack().size() >= 1) {
        SetButtonLabel(
            bestBackPrice1,
            runner.getEx().getAvailableToBack()[0].getPrice(),
            runner.getEx().getAvailableToBack()[0].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestBackPrice1);
    }

    if (marketOpen && runner.getEx().getAvailableToLay().size() >= 1) {
        SetButtonLabel(
            bestLayPrice1,
            runner.getEx().getAvailableToLay()[0].getPrice(),
            runner.getEx().getAvailableToLay()[0].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestLayPrice1);
    }

    if (marketOpen && runner.getEx().getAvailableToLay().size() >= 2) {
        SetButtonLabel(
            bestLayPrice2,
            runner.getEx().getAvailableToLay()[1].getPrice(),
            runner.getEx().getAvailableToLay()[1].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestLayPrice2);
    }

    if (marketOpen && runner.getEx().getAvailableToLay().size() >= 3) {
        SetButtonLabel(
            bestLayPrice3,
            runner.getEx().getAvailableToLay()[2].getPrice(),
            runner.getEx().getAvailableToLay()[2].getSize(),
            wxCurrencySymbol
        );
    } else {
        ResetButton(bestLayPrice3);
    }

}

void RunnerRow::SetPendingPlaceInstruction(const greentop::PlaceInstruction& placeInstruction) {

    double diff = 0;

    if (runner.getSelectionId() == placeInstruction.getSelectionId()) {

        diff = placeInstruction.getLimitOrder().getSize() * (placeInstruction.getLimitOrder().getPrice() - 1);
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

        std::ostringstream profitLabelStream;
        profitLabelStream << "» " << currencySymbol << std::fixed << std::setprecision(2) << std::abs(pendingProfitAndLoss);
        wxString profitLabel((profitLabelStream.str()).c_str(), wxConvUTF8);
        pendingProfit->SetLabel(profitLabel);

        if (pendingProfitAndLoss > 0) {
            pendingProfit->SetForegroundColour(wxColour(0, 128, 0));
        } else {
            pendingProfit->SetForegroundColour(wxColour("RED"));
        }

    } else {
        pendingProfit->SetLabel("");
    }

}

void RunnerRow::SetButtonLabel(PriceButton* button, const double price, const double size, const wxString& currency) const {

    wxString label = wxString::Format(wxT("%.2f\n"), price) + currency +
        wxString::Format(wxT("%.0f"), size);
    button->SetLabel(label);
    button->SetPrice(price);

}

void RunnerRow::ResetButton(PriceButton* button) const {

    button->SetLabel("");

    double price;
    if (button->GetSide() == greentop::Side::BACK) {
        price = 100;
    } else {
        price = 1.01;
    }
    button->SetPrice(price);
}

void RunnerRow::OnClickChart(wxEvent& event) {

    dialog::PriceHistory priceHistory(NULL, wxID_ANY, "Price History");
    priceHistory.SetLastPriceTraded(lastPriceTraded);
    priceHistory.SetRunner(market, runner);
    priceHistory.ShowModal();

}

}
