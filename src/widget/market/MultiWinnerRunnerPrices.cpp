/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */

#include "widget/market/MultiWinnerRunnerPrices.h"

namespace greenthumb {
namespace widget {
namespace market {

MultiWinnerRunnerPrices::MultiWinnerRunnerPrices(wxWindow* parent) : RunnerPrices(parent) {
    CreateChartButton(parent);
    CreateRunnerName(parent);

    sizer = parent->GetSizer();

    wxPanel* profitContainer = new wxPanel(parent, wxID_ANY);
    profitContainer->SetMinSize(wxSize(150, -1));
    wxSizer* profitContainerSizer = new wxBoxSizer(wxHORIZONTAL);
    profitContainer->SetSizer(profitContainerSizer);
    profitIfWinText = new wxStaticText(profitContainer, wxID_ANY, wxEmptyString);
    profitContainerSizer->Add(profitIfWinText);
    profitSeparator = new wxStaticText(profitContainer, wxID_ANY, wxEmptyString);
    profitContainerSizer->Add(profitSeparator);
    profitIfLoseText = new wxStaticText(profitContainer, wxID_ANY, wxEmptyString);
    profitContainerSizer->Add(profitIfLoseText);
    sizer->Add(profitContainer, 0, wxALIGN_CENTRE_VERTICAL);

    wxPanel* pendingContainer = new wxPanel(parent, wxID_ANY);
    pendingContainer->SetMinSize(wxSize(150, -1));
    wxSizer* pendingContainerSizer = new wxBoxSizer(wxHORIZONTAL);
    pendingContainer->SetSizer(pendingContainerSizer);
    pendingIfWinText = new wxStaticText(pendingContainer, wxID_ANY, wxEmptyString);
    pendingContainerSizer->Add(pendingIfWinText);
    pendingSeparatorText = new wxStaticText(pendingContainer, wxID_ANY, wxEmptyString);
    pendingContainerSizer->Add(pendingSeparatorText);
    pendingIfLoseText = new wxStaticText(pendingContainer, wxID_ANY, wxEmptyString);
    pendingContainerSizer->Add(pendingIfLoseText);
    sizer->Add(pendingContainer, 0, wxALIGN_CENTRE_VERTICAL);

    CreatePricesButtons(parent);
}

void MultiWinnerRunnerPrices::SetPendingPlaceInstruction(const greentop::sport::PlaceInstruction& placeInstruction) {
    if (GetRunner().getSelectionId() == placeInstruction.getSelectionId()) {                
        double winDiff = placeInstruction.getLimitOrder().getSize() *
            (placeInstruction.getLimitOrder().getPrice() - 1);
        double loseDiff = placeInstruction.getLimitOrder().getSize() * -1;
        if (placeInstruction.getSide() == greentop::sport::Side::LAY) {
            winDiff *= -1;
            loseDiff *= -1; 
        }
        if (winDiff > 0.001 || winDiff < -0.001) {
            double pendingProfitAndLossIfWin = profitAndLossIfWin + winDiff;
            double pendingProfitAndLossIfLose = profitAndLossIfLose + loseDiff;

            wxString profitLabel = wxString("» ", wxConvUTF8) + currencySymbol
                + wxString::Format("%.2f", std::abs(pendingProfitAndLossIfWin));
            pendingIfWinText->SetLabel(profitLabel);
            if (pendingProfitAndLossIfWin > 0) {
                pendingIfWinText->SetForegroundColour(wxColour(0, 128, 0));
            } else {
                pendingIfWinText->SetForegroundColour(wxColour("RED"));
            }
            pendingSeparatorText->SetLabel(", ");
            profitLabel = currencySymbol
                + wxString::Format("%.2f", std::abs(pendingProfitAndLossIfLose));
            pendingIfLoseText->SetLabel(profitLabel);
            if (pendingProfitAndLossIfLose > 0) {
                pendingIfLoseText->SetForegroundColour(wxColour(0, 128, 0));
            } else {
                pendingIfLoseText->SetForegroundColour(wxColour("RED"));
            }
        } else {
            pendingIfWinText->SetLabel("");
            pendingSeparatorText->SetLabel("");
            pendingIfLoseText->SetLabel("");
        }
    } else {
        pendingIfWinText->SetLabel("");
        pendingSeparatorText->SetLabel("");
        pendingIfLoseText->SetLabel("");
    }
}

/* void MultiWinnerRunnerPrices::SetProfit(double handicap, double profitIfWin, double profitIfLose) {
    int64_t scaledHandicap = handicap * scaleFactor;

    profits[scaledHandicap] = std::pair<double, double>(profitIfWin, profitIfLose);

    if (scaledHandicap == this->handicap) {
        UpdateProfitAndLossIfWin();
    }
} */

void MultiWinnerRunnerPrices::SetProfitLabel(wxStaticText* profitText, double profit) const {
    profitText->SetLabel(currencySymbol + wxString::Format("%.2f", std::abs(profit)));

    if (profit > 0) {
        profitText->SetForegroundColour(wxColour(0, 128, 0));
    } else {
        profitText->SetForegroundColour(wxColour("RED"));
    }
}

void MultiWinnerRunnerPrices::UpdateProfitAndLossIfWin() {
    auto it = profits.find(handicap);
    if (it != profits.end()) {
        profitAndLossIfWin = it->second.first;
        profitAndLossIfLose = it->second.second;

        if (profitAndLossIfWin > 0.001 || profitAndLossIfWin < -0.001 || profitAndLossIfLose > 0.001 || profitAndLossIfLose < -0.001) {
            SetProfitLabel(profitIfWinText, profitAndLossIfWin);
            profitSeparator->SetLabel(", ");
            SetProfitLabel(profitIfLoseText, profitAndLossIfLose);
        } else {
            profitIfWinText->SetLabel(wxEmptyString);
            profitSeparator->SetLabel(wxEmptyString);
            profitIfLoseText->SetLabel(wxEmptyString);
        }
    } else {
        profitIfWinText->SetLabel(wxEmptyString);
        profitSeparator->SetLabel(wxEmptyString);
        profitIfLoseText->SetLabel(wxEmptyString);
    }
    sizer->Layout();
}

}
}
}
