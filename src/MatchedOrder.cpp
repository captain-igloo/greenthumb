/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <iomanip>
#include <sstream>

#include "MatchedOrder.h"
#include "Util.h"

#include "entity/Config.h"

namespace greenthumb {

MatchedOrder::MatchedOrder(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : CurrentOrder(parent, id, pos, size, style, name) {

    wxFlexGridSizer* sizer = new wxFlexGridSizer(6, 5, 5);
    sizer->AddGrowableCol(0, 1);
    SetSizer(sizer);

    runnerName = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    runnerName->SetMinSize(wxSize(150, -1));

    sizer->Add(runnerName, 0, wxALIGN_CENTRE_VERTICAL);

    price = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    sizer->Add(price, 0, wxALIGN_CENTRE_VERTICAL);

    wxStaticText* stakeLabel = new wxStaticText(this, wxID_ANY, "Stake: ");
    sizer->Add(stakeLabel, 0, wxALIGN_CENTRE_VERTICAL);
    stake = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(stake, 0, wxALIGN_CENTRE_VERTICAL);

    profitOrLiabilityLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
    profitOrLiabilityLabel->SetMinSize(wxSize(75, -1));
    sizer->Add(profitOrLiabilityLabel, 0, wxALIGN_CENTRE_VERTICAL);
    profitOrLiability = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(profitOrLiability, 0, wxALIGN_CENTRE_VERTICAL);

}

void MatchedOrder::SetCurrentOrderSummary(const greentop::CurrentOrderSummary& cos) {
    currentOrderSummary = cos;

    if (market.HasRunner(currentOrderSummary.getSelectionId())) {
        greentop::RunnerCatalog runner = market.GetRunner(currentOrderSummary.getSelectionId());

        wxString label(runner.getRunnerName().c_str(), wxConvUTF8);
        runnerName->SetLabel(currentOrderSummary.getSide().getValue() + " " + label);

        std::ostringstream priceLabelStream;
        double avPriceMatched = currentOrderSummary.getAveragePriceMatched();
        priceLabelStream << std::fixed << std::setprecision(2) << avPriceMatched;
        wxString priceLabel(priceLabelStream.str().c_str(), wxConvUTF8);
        price->SetLabel(priceLabel);

        std::string currencySymbol = GetCurrencySymbol(entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));

        std::ostringstream stakeLabelStream;
        double sizeMatched = 0;
        greentop::Optional<double> optionalSizeMatched = currentOrderSummary.getSizeMatched();
        if (optionalSizeMatched.isValid()) {
            sizeMatched = optionalSizeMatched.getValue();
        }
        stakeLabelStream << currencySymbol << std::fixed << std::setprecision(2) << sizeMatched;
        wxString stakeLabel(stakeLabelStream.str().c_str(), wxConvUTF8);

        stake->SetLabel(stakeLabel);

        if (currentOrderSummary.getSide() == greentop::Side::BACK) {
            profitOrLiabilityLabel->SetLabel(_("Profit: "));
            SetBackgroundColour(wxColour(227, 235, 255));
        } else if (currentOrderSummary.getSide() == greentop::Side::LAY) {
            profitOrLiabilityLabel->SetLabel(_("Liability: "));
            SetBackgroundColour(wxColour(255, 224, 255));
        }

        UpdateProfitOrLiability();

    }
}

void MatchedOrder::UpdateProfitOrLiability() {

    std::string currencySymbol = GetCurrencySymbol(entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));

    std::ostringstream profitOrLiabilityStream;

    double profit;

    double sizeMatched = 0;
    greentop::Optional<double> optionalSizeMatched = currentOrderSummary.getSizeMatched();
    if (optionalSizeMatched.isValid()) {
        sizeMatched = optionalSizeMatched.getValue();
    }

    if (currentOrderSummary.getSide() == greentop::Side::BACK) {
        profit = sizeMatched * (currentOrderSummary.getPriceSize().getPrice() - 1);
    } else if (currentOrderSummary.getSide() == greentop::Side::LAY) {
        profit = sizeMatched * (currentOrderSummary.getPriceSize().getPrice() - 1);
    }

    profitOrLiabilityStream << currencySymbol << std::fixed << std::setprecision(2) << profit;

    wxString label(profitOrLiabilityStream.str().c_str(), wxConvUTF8);
    profitOrLiability->SetLabel(label);

}

}
