/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */

#include "widget/market/SingleWinnerRunnerPrices.h"

namespace greenthumb {
namespace widget {
namespace market {

SingleWinnerRunnerPrices::SingleWinnerRunnerPrices(wxWindow* parent) : RunnerPrices(parent) {
    CreateChartButton(parent);
    CreateRunnerName(parent);

    wxSizer* sizer = parent->GetSizer();

    profitAndLossIfWinText = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    profitAndLossIfWinText->SetMinSize(wxSize(100, -1));
    sizer->Add(profitAndLossIfWinText, 0, wxALIGN_CENTRE_VERTICAL);

    pendingProfit = new wxStaticText(parent, wxID_ANY, wxEmptyString);
    pendingProfit->SetMinSize(wxSize(100, -1));
    sizer->Add(pendingProfit, 0, wxALIGN_CENTRE_VERTICAL);

    CreatePricesButtons(parent);
}

void SingleWinnerRunnerPrices::UpdateProfitAndLossIfWin() {
    auto it = profits.find(handicap);
    if (it != profits.end()) {
        profitAndLossIfWin = it->second.first;

        if (profitAndLossIfWin > 0.001 || profitAndLossIfWin < -0.001) {
            wxString profitLabel = currencySymbol +
                wxString::Format("%.2f", std::abs(profitAndLossIfWin));

            profitAndLossIfWinText->SetLabel(profitLabel);

            if (profitAndLossIfWin > 0) {
                profitAndLossIfWinText->SetForegroundColour(wxColour(0, 128, 0));
            } else {
                profitAndLossIfWinText->SetForegroundColour(wxColour("RED"));
            }
        } else {
            profitAndLossIfWinText->SetLabel("");
        }
    } else {
        profitAndLossIfWinText->SetLabel("");
    }
}

void SingleWinnerRunnerPrices::SetPendingPlaceInstruction(const greentop::PlaceInstruction& placeInstruction) {
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

}
}
}
