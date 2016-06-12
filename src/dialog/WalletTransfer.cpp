#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>

#include "dialog/WalletTransfer.h"
#include "entity/Config.h"
#include "worker/GetAccountFunds.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace dialog {

WalletTransfer::WalletTransfer(wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
    wxDialog(parent, id, title, pos, size, style, name), transferFundsRunning(false), workerManager(this) {

    int border = 10;
    int borderFlags = wxTOP | wxRIGHT | wxLEFT;
    int columnOneWidth = 150;

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    /* wxString wallets[] = {
        wxT("UK"), wxT("Australian")
    }; */

    wxStaticText* heading = new wxStaticText(this, wxID_ANY, "Transfer Funds between your Main and Australian Licence Wallets");
    vbox->Add(heading, 0, wxEXPAND | borderFlags, border);

    wxStaticBoxSizer* fundsSourceSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Select source of funds");

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    srcUkWallet = new wxRadioButton(fundsSourceSizer->GetStaticBox(), wxID_ANY,
        "Main wallet", wxDefaultPosition, wxSize(columnOneWidth, -1), wxRB_GROUP);
    hbox->Add(srcUkWallet);
    ukBalance = new wxStaticText(fundsSourceSizer->GetStaticBox(), wxID_ANY, "");
    hbox->Add(ukBalance, 0, wxALIGN_CENTRE_VERTICAL);
    fundsSourceSizer->Add(hbox, 1, wxEXPAND);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxRadioButton* srcAusWallet = new wxRadioButton(fundsSourceSizer->GetStaticBox(), wxID_ANY,
        "Australian wallet", wxDefaultPosition, wxSize(columnOneWidth, -1));
    hbox->Add(srcAusWallet);
    ausBalance = new wxStaticText(fundsSourceSizer->GetStaticBox(), wxID_ANY, "");
    hbox->Add(ausBalance, 0, wxALIGN_CENTRE_VERTICAL);
    fundsSourceSizer->Add(hbox, 1, wxEXPAND);

    vbox->Add(fundsSourceSizer, 0, wxEXPAND | borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* amountLabel = new wxStaticText(this, wxID_ANY, "Amount:", wxDefaultPosition,
        wxSize(columnOneWidth, -1));
    hbox->Add(amountLabel, 0, wxALIGN_CENTRE_VERTICAL);
    wxFloatingPointValidator<float> numberValidator(2);
    amount = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, numberValidator);
    hbox->Add(amount, 1, wxEXPAND);
    vbox->Add(hbox, 0, wxEXPAND |  borderFlags, border);

    hbox = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* destLabel = new wxStaticText(this, wxID_ANY, "Destination wallet:", wxDefaultPosition,
        wxSize(columnOneWidth, -1));
    hbox->Add(destLabel);
    destinationWalletText = new wxStaticText(this, wxID_ANY, "Australian");
    hbox->Add(destinationWalletText);
    vbox->Add(hbox, 0, wxEXPAND | borderFlags, border);

    // wxChoice* dest = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, wallets);
    // vbox->Add(dest, 0, wxEXPAND | borderFlags, border);

    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxBOTTOM | wxALIGN_RIGHT, border);

        okButton = FindWindow(wxID_OK);
        okButton->Disable();
        cancelButton = FindWindow(wxID_CANCEL);
        // wxWindow* cancelButton = FindWindow(wxID_CANCEL);
    }

    SetSizer(vbox);
    vbox->Fit(this);

    Bind(worker::GET_ACCOUNT_FUNDS_UK, &WalletTransfer::OnGetAccountFundsUk, this);
    workerManager.Bind(worker::GET_ACCOUNT_FUNDS_UK);
    Bind(worker::GET_ACCOUNT_FUNDS_AUS, &WalletTransfer::OnGetAccountFundsAus, this);
    workerManager.Bind(worker::GET_ACCOUNT_FUNDS_AUS);
    Bind(worker::TRANSFER_FUNDS, &WalletTransfer::OnTransfer, this);
    workerManager.Bind(worker::TRANSFER_FUNDS);

    Bind(wxEVT_BUTTON, &WalletTransfer::OnOk, this, wxID_OK);
    Bind(wxEVT_RADIOBUTTON, &WalletTransfer::OnChangeWallet, this, wxID_ANY);
    Bind(wxEVT_TEXT, &WalletTransfer::OnTextChange, this, wxID_ANY);

    GetAccountFunds();

}

void WalletTransfer::GetAccountFunds() {

    workerManager.RunWorker(new worker::GetAccountFunds(&workerManager, greentop::Wallet::UK));

    workerManager.RunWorker(new worker::GetAccountFunds(&workerManager, greentop::Wallet::AUSTRALIAN));

}

void WalletTransfer::OnGetAccountFundsUk(const wxThreadEvent& event) {
    OnGetAccountFunds(event, ukBalance);
}

void WalletTransfer::OnGetAccountFundsAus(const wxThreadEvent& event) {
    OnGetAccountFunds(event, ausBalance);
}

void WalletTransfer::OnGetAccountFunds(const wxThreadEvent& event, wxStaticText* balance) {

    std::string currencySymbol = GreenThumb::GetCurrencySymbol(
        entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));
    wxString wxCurrencySymbol(currencySymbol.c_str(), wxConvUTF8);

    greentop::AccountFundsResponse afr = event.GetPayload<greentop::AccountFundsResponse>();

    if (afr.isSuccess()) {
        balance->SetLabel(wxCurrencySymbol + wxString::Format(wxT("%.2f"), afr.getAvailableToBetBalance()));
    }

}

/* bool WalletTransfer::IsValid() {

    if (okButton != NULL) {

        amount->GetValue();

    }
    return true;
} */

void WalletTransfer::OnOk(wxCommandEvent& event) {

    greentop::Wallet from;
    greentop::Wallet to;

    if (srcUkWallet->GetValue()) {
        from = greentop::Wallet::UK;
        to = greentop::Wallet::AUSTRALIAN;
    } else {
        from = greentop::Wallet::AUSTRALIAN;
        to = greentop::Wallet::UK;
    }

    double dblAmount;
    if (amount->GetValue().ToDouble(&dblAmount)) {

        okButton->Disable();
        amount->SetValue("");
        amount->Disable();

        greentop::TransferFundsRequest req(from, to, dblAmount);

        transferFundsWorker = new worker::TransferFunds(&workerManager, req);
        workerManager.RunWorker(transferFundsWorker);
    } else {
        wxLogError("Failed to convert " + amount->GetValue());
    }

}

void WalletTransfer::OnTransfer(wxThreadEvent& event) {
    amount->Enable();
    GetAccountFunds();
}

void WalletTransfer::OnChangeWallet(wxCommandEvent& event) {

    if (srcUkWallet->GetValue()) {
        // src is UK so dest is aussie
        destinationWalletText->SetLabel("Australian");
    } else {
        // src is aussie so dest is UK
        destinationWalletText->SetLabel("Main");
    }
}

void WalletTransfer::OnTextChange(wxCommandEvent& event) {
    if (Validate()) {
        okButton->Enable();
    } else {
        okButton->Disable();
    }
}

bool WalletTransfer::Validate() {
    return !amount->GetValue().IsEmpty();
}

}
}

