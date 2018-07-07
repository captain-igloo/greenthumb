/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <wx/checkbox.h>

#include "dialog/LoginDialog.h"
#include "entity/Config.h"
#include "worker/Login.h"
#include "ArtProvider.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace dialog {

LoginDialog::LoginDialog(wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
    wxDialog(parent, id, title, pos, size, style, name), workerManager(this) {

    int border = 10;
    int borderFlags = wxTOP | wxRIGHT | wxLEFT;

    wxIcon greenthumbIcon;
    greenthumbIcon.CopyFromBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::GREENTHUMB));
    SetIcon(greenthumbIcon);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText* message = new wxStaticText(this, wxID_ANY, "Enter your Betfair username and password.");
    vbox->Add(message, 0, wxEXPAND | borderFlags, border);

    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    vbox->Add(hbox1, 0, wxEXPAND | borderFlags, border);

    wxStaticText* appKeyLabel = new wxStaticText(this, wxID_ANY, "Application key:", wxDefaultPosition, wxSize(100, -1));
    hbox1->Add(appKeyLabel, 0, wxALIGN_CENTRE_VERTICAL);

    appKey = new wxTextCtrl(this, wxID_ANY);
    appKey->SetMinSize(wxSize(300, -1));
    hbox1->Add(appKey, 1, wxEXPAND);

    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    vbox->Add(hbox2, 0, wxEXPAND | borderFlags, border);

    wxStaticText* usernameLabel = new wxStaticText(this, wxID_ANY, "Username:", wxDefaultPosition, wxSize(100, -1));
    hbox2->Add(usernameLabel, 0, wxALIGN_CENTRE_VERTICAL);

    username = new wxTextCtrl(this, wxID_ANY);
    username->SetMinSize(wxSize(300, -1));
    hbox2->Add(username, 1, wxEXPAND);

    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
    vbox->Add(hbox3, 0, wxEXPAND | borderFlags, border);
    wxStaticText* passwordLabel = new wxStaticText(this, wxID_ANY, "Password:", wxDefaultPosition, wxSize(100, -1));
    hbox3->Add(passwordLabel, 0, wxALIGN_CENTRE_VERTICAL);

    password = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    password->SetMinSize(wxSize(300, -1));
    hbox3->Add(password, 1, wxEXPAND);

    wxString savedAppKey = entity::Config::GetConfigValue<wxString>("applicationKey", "");
    wxString savedUsername = entity::Config::GetConfigValue<wxString>("betfairUsername", "");
    wxString savedPassword = entity::Config::GetConfigValue<wxString>("betfairPassword", "");

    if (savedAppKey != "") {
        appKey->SetValue(savedAppKey);
    }

    if (savedUsername != "" && savedPassword != "") {
        username->SetValue(savedUsername);
        password->SetValue(savedPassword);
    }

    wxBoxSizer* hbox4 = new wxBoxSizer(wxHORIZONTAL);
    remember = new wxCheckBox(this, wxID_ANY, "Remember password");
    hbox4->Add(remember);

    bool rememberBetfairPassword = entity::Config::GetConfigValue("rememberBetfairPassword", false);
    remember->SetValue(rememberBetfairPassword);

    vbox->Add(hbox4, 0, wxEXPAND | borderFlags, border);

    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxTOP | wxBOTTOM | wxALIGN_RIGHT, border);

        okButton = FindWindow(wxID_OK);
        cancelButton = FindWindow(wxID_CANCEL);
    }

    SetSizer(vbox);
    vbox->Fit(this);

    OnTextChange();

    Bind(wxEVT_BUTTON, &LoginDialog::OnOk, this, wxID_OK);
    Bind(wxEVT_TEXT, &LoginDialog::OnTextChange, this, wxID_ANY);

    Bind(worker::LOGIN, &LoginDialog::OnLogin, this, wxID_ANY);
    workerManager.Bind(worker::LOGIN);
}

void LoginDialog::OnOk(const wxCommandEvent& event) {

    entity::Config::SetConfigValue("applicationKey", appKey->GetValue());

    entity::Config::SetConfigValue("rememberBetfairPassword", remember->GetValue());
    wxString usernameToSave = "";
    wxString passwordToSave = "";
    if (remember->GetValue()) {
        usernameToSave = username->GetValue();
        passwordToSave = password->GetValue();
    }
    entity::Config::SetConfigValue("betfairUsername", usernameToSave);
    entity::Config::SetConfigValue("betfairPassword", passwordToSave);

    if (okButton != NULL) {
        okButton->Disable();
    }
    if (cancelButton != NULL) {
        cancelButton->Disable();
    }

    workerManager.RunWorker(new worker::Login(&workerManager, std::string(appKey->GetValue().mb_str()),
        std::string(username->GetValue().mb_str()), std::string(password->GetValue().mb_str())));

}

void LoginDialog::OnTextChange() {
    if (okButton != NULL) {
        if (username->GetValue() != wxEmptyString && password->GetValue() != wxEmptyString &&
            appKey->GetValue() != wxEmptyString) {
            okButton->Enable();
        } else {
            okButton->Disable();
        }
    }
}

void LoginDialog::OnTextChange(const wxCommandEvent& event) {
    OnTextChange();
}

void LoginDialog::OnLogin(const wxThreadEvent& event) {

    if (event.GetPayload<bool>()) {
        EndModal(wxID_OK);
    }

    if (okButton != NULL) {
        okButton->Enable();
    }
    if (cancelButton != NULL) {
        cancelButton->Enable();
    }

}

}
}
