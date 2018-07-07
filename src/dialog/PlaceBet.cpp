/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>

#include <iomanip>
#include <sstream>

#include "dialog/PlaceBet.h"
#include "entity/Config.h"
#include "worker/PlaceOrders.h"

#include "GreenThumb.h"
#include "Util.h"

namespace greenthumb {
namespace dialog {

wxDEFINE_EVENT(PLACE_ORDER_PENDING, wxCommandEvent);

PlaceBet::PlaceBet(wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
    wxDialog(parent, id, title, pos, size, style, name), workerManager(this) {

    int border = 10;
    int borderFlags = wxTOP | wxRIGHT | wxLEFT;
    int columnOneWidth = 100;
    int columnTwoWidth = 200;
    wxString currencySymbol = GetCurrencySymbol(
        entity::Config::GetConfigValue<wxString>("accountCurrency", "?")
    );
    int defaultStake = entity::Config::GetConfigValue<int>("defaultStake", 100);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    marketLabel = new wxStaticText(this, wxID_ANY, _(""));
    vbox->Add(marketLabel, 0, wxEXPAND | borderFlags, border);

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* selectionLabel = new wxStaticText(
        this,
        wxID_ANY,
        _("Selection:"),
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(selectionLabel, 0, wxALIGN_CENTRE_VERTICAL);

    selection = new wxStaticText(this, wxID_ANY, _(""));
    hbox->Add(selection, 1, wxEXPAND);

    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* oddsLabel = new wxStaticText(
        this,
        wxID_ANY,
        _("Odds:"),
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(oddsLabel, 0, wxALIGN_CENTRE_VERTICAL);

    oddsSpin = new OddsSpinCtrl(this, wxID_ANY);
    oddsSpin->SetMinSize(wxSize(columnTwoWidth, -1));
    hbox->Add(oddsSpin, 1, wxEXPAND);
    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* stakeLabel = new wxStaticText(
        this,
        wxID_ANY,
        _("Stake (") + currencySymbol + _("):"),
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(stakeLabel, 0, wxALIGN_CENTRE_VERTICAL);
    wxFloatingPointValidator<float> numberValidator(2);
    stakeSpin = new wxTextCtrl(
        this,
        wxID_ANY,
        std::to_string(defaultStake),
        wxDefaultPosition,
        wxDefaultSize,
        0,
        numberValidator
    );
    stakeSpin->SetMinSize(wxSize(columnTwoWidth, -1));
    hbox->Add(stakeSpin, 1, wxEXPAND);
    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* persistLabel = new wxStaticText(
        this,
        wxID_ANY,
        "At In-Play:",
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(persistLabel, 0, wxALIGN_CENTRE_VERTICAL);
    persistKeep = new wxRadioButton(
        this,
        wxID_ANY,
        "Keep",
        wxDefaultPosition,
        wxDefaultSize,
        wxRB_GROUP
    );
    persistKeep->SetMinSize(wxSize(columnTwoWidth / 2, -1));
    hbox->Add(persistKeep, 0, wxALIGN_CENTRE_VERTICAL);
    persistCancel = new wxRadioButton(this, wxID_ANY, "Cancel");
    persistCancel->SetValue(true);
    persistCancel->SetMinSize(wxSize(columnTwoWidth / 2, -1));
    hbox->Add(persistCancel, 0, wxALIGN_CENTRE_VERTICAL);
    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* profitLabel = new wxStaticText(
        this,
        wxID_ANY,
        _("Profit (") + currencySymbol + _("):"),
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(profitLabel, 0, wxALIGN_CENTRE_VERTICAL);

    profit = new wxStaticText(this, wxID_ANY, _("0"));
    hbox->Add(profit, 0, wxALIGN_CENTRE_VERTICAL);

    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* liabilityLabel = new wxStaticText(
        this,
        wxID_ANY,
        _("Liability (") + currencySymbol + _("):"),
        wxDefaultPosition,
        wxSize(columnOneWidth, -1)
    );
    hbox->Add(liabilityLabel, 0, wxALIGN_CENTRE_VERTICAL);

    liability = new wxStaticText(this, wxID_ANY, _("0"));
    hbox->Add(liability, 0, wxALIGN_CENTRE_VERTICAL);

    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxTOP | wxBOTTOM | wxALIGN_RIGHT, border);

        backLayButton = FindWindow(wxID_OK);
        cancelButton = FindWindow(wxID_CANCEL);
    }

    SetSizer(vbox);
    vbox->Fit(this);

    long extraStyle = GetExtraStyle();
    SetExtraStyle(extraStyle - wxWS_EX_BLOCK_EVENTS);

    Bind(wxEVT_TEXT, &PlaceBet::OnTextChange, this, wxID_ANY);
    Bind(wxEVT_SPINCTRLDOUBLE, &PlaceBet::OnSpinChange, this, wxID_ANY);
    Bind(wxEVT_BUTTON, &PlaceBet::OnSubmit, this, wxID_OK);
    Bind(wxEVT_BUTTON, &PlaceBet::OnCancel, this, wxID_CANCEL);
    Bind(worker::PLACE_ORDERS, &PlaceBet::OnPlaceBet, this, wxID_ANY);
    workerManager.Bind(worker::PLACE_ORDERS);

}

void PlaceBet::SetPlaceInstruction(const std::string& runnerName, const greentop::PlaceInstruction& placeInstruction) {

    side = placeInstruction.getSide();
    selectionId = placeInstruction.getSelectionId();
    handicap = placeInstruction.getHandicap();

    oddsSpin->SetValue(placeInstruction.getLimitOrder().getPrice());

    wxString runner(runnerName.c_str(), wxConvUTF8);

    if (market.GetMarketCatalogue().getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE) {
        wxString sign = "";
        if (placeInstruction.getHandicap() > 0) {
            sign = "+";
        }
        runner = runner + " " + sign + wxString::Format(wxT("%.1f"), placeInstruction.getHandicap().getValue());
    }

    selection->SetLabel(runner);

    UpdateProfitAndLiability();
}

void PlaceBet::SetMarket(const entity::Market& market, const std::string& fullMarketName) {
    this->market = market;
    wxString label(fullMarketName.c_str(), wxConvUTF8);
    marketLabel->SetLabel(label);
}

void PlaceBet::UpdateProfitAndLiability() {

    double stake;
    if (stakeSpin->GetValue().ToDouble(&stake)) {

        std::ostringstream profitLabelStream;
        std::ostringstream liabilityLabelStream;

        if (side == greentop::Side::BACK) {

            profitLabelStream << std::fixed << std::setprecision(2) <<
                ((oddsSpin->GetValue() * stake) - stake);
            wxString profitLabel(profitLabelStream.str().c_str(), wxConvUTF8);

            profit->SetLabel(profitLabel);

            liabilityLabelStream << std::fixed << std::setprecision(2) << stake;
            wxString liabilityLabel(liabilityLabelStream.str().c_str(), wxConvUTF8);

            liability->SetLabel(liabilityLabel);

        } else {

            profitLabelStream << std::fixed << std::setprecision(2) << stake;
            wxString profitLabel(profitLabelStream.str().c_str(), wxConvUTF8);

            profit->SetLabel(profitLabel);

            liabilityLabelStream << std::fixed << std::setprecision(2) <<
                ((oddsSpin->GetValue() - 1) * stake);
            wxString liabilityLabel(liabilityLabelStream.str().c_str(), wxConvUTF8);

            liability->SetLabel(liabilityLabel);

        }

        PlaceOrderPending(stake, oddsSpin->GetValue());
    } else {
        profit->SetLabel("");
        liability->SetLabel("");
        PlaceOrderPending(0, -1);
        wxLogStatus("Failed to convert: " + stakeSpin->GetValue());
    }
}

void PlaceBet::PlaceOrderPending(const double stake, const double odds) {
    greentop::LimitOrder limitOrder(stake, odds, greentop::PersistenceType::LAPSE);
    pendingPlaceInstruction.setSide(side);
    pendingPlaceInstruction.setSelectionId(selectionId);
    pendingPlaceInstruction.setLimitOrder(limitOrder);

    wxCommandEvent event(PLACE_ORDER_PENDING);
    event.SetClientData(&pendingPlaceInstruction);
    AddPendingEvent(event);
}


void PlaceBet::OnSpinChange(const wxEvent& spinEvent) {
    if (backLayButton->IsEnabled()) {
        UpdateProfitAndLiability();
    }
}

void PlaceBet::OnSubmit(const wxCommandEvent& event) {
    backLayButton->Disable();
    cancelButton->Disable();

    double stake;
    if (stakeSpin->GetValue().ToDouble(&stake)) {
        greentop::OrderType orderType(greentop::OrderType::LIMIT);
        greentop::PersistenceType persistenceType(
            persistKeep->GetValue() ? greentop::PersistenceType::PERSIST : greentop::PersistenceType::LAPSE
        );
        greentop::LimitOrder limitOrder(stake, oddsSpin->GetValue(), persistenceType);
        greentop::PlaceInstruction placeInstruction(
            orderType,
            selectionId,
            handicap,
            side,
            limitOrder
        );

        std::vector<greentop::PlaceInstruction> instructions;
        instructions.push_back(placeInstruction);

        greentop::PlaceOrdersRequest placeOrdersRequest(
            market.GetMarketCatalogue().getMarketId(),
            instructions
        );

        workerManager.RunWorker(new worker::PlaceOrders(&workerManager, placeOrdersRequest));
    } else {
        wxLogError("Failed to convert: " + stakeSpin->GetValue());
    }

    PlaceOrderPending(0, -1);

}

void PlaceBet::OnCancel(wxCommandEvent& event) {
    PlaceOrderPending(0, -1);
    event.Skip();
}

void PlaceBet::OnPlaceBet(wxThreadEvent& event) {
    event.Skip();
    Destroy();
}

void PlaceBet::OnTextChange(wxCommandEvent& event) {
    UpdateProfitAndLiability();
    if (stakeSpin->GetValue() == "") {
        backLayButton->Disable();
    } else {
        backLayButton->Enable();
    }
}

}
}
