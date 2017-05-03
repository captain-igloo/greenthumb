#include <wx/wx.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "dialog/Settings.h"
#include "entity/Config.h"

namespace greenthumb {
namespace dialog {

const uint32_t Settings::ACCOUNT_PAGE_SIZE = 20;
const uint32_t Settings::DEFAULT_STAKE = 100;
const uint32_t Settings::REFRESH_INTERVAL = 60;

Settings::Settings(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxDialog(parent, id, title, pos, size, style, name) {

    int border = 10;

    int accountPageSize = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_ACCOUNT_PAGE_SIZE,
        ACCOUNT_PAGE_SIZE
    );
    int stake = entity::Config::GetConfigValue<int>("defaultStake", DEFAULT_STAKE);
    int interval = entity::Config::GetConfigValue<int>(
        entity::Config::KEY_REFRESH_INTERVAL,
        REFRESH_INTERVAL
    );

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    wxGridSizer* gridSizer = new wxGridSizer(2, 5, 5);

    wxStaticText* accountRowsPerPageLabel = new wxStaticText(this, wxID_ANY, "Account - rows per page");
    gridSizer->Add(accountRowsPerPageLabel, 0, wxALIGN_CENTRE_VERTICAL, border);
    accountRowsPerPage = new wxSpinCtrl(this, wxID_ANY, std::to_string(accountPageSize));
    gridSizer->Add(accountRowsPerPage, 0, wxALIGN_CENTRE_VERTICAL, border);

    wxStaticText* defaultStakeLabel = new wxStaticText(this, wxID_ANY, "Default stake");
    gridSizer->Add(defaultStakeLabel, 0, wxALIGN_CENTRE_VERTICAL, border);
    defaultStake = new wxSpinCtrl(this, wxID_ANY, std::to_string(stake));
    gridSizer->Add(defaultStake, 0, wxALIGN_CENTRE_VERTICAL, border);

    wxStaticText* refreshIntervalLabel = new wxStaticText(this, wxID_ANY, "Refresh interval (seconds)");
    gridSizer->Add(refreshIntervalLabel, 0, wxALIGN_CENTRE_VERTICAL, border);
    refreshInterval = new wxSpinCtrl(this, wxID_ANY, std::to_string(interval));
    gridSizer->Add(refreshInterval, 0, wxALIGN_CENTRE_VERTICAL, border);

    sizer->Add(gridSizer, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, border);

    wxSizer* buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    if (buttonSizer) {
        sizer->Add(buttonSizer, 0, wxTOP | wxBOTTOM | wxALIGN_RIGHT, border);
    }
    sizer->Fit(this);

    Bind(wxEVT_BUTTON, &Settings::OnSave, this, wxID_OK);
    Bind(wxEVT_BUTTON, &Settings::OnCancel, this, wxID_CANCEL);
}

void Settings::OnSave(const wxCommandEvent& event) {
    int rowsPerPage = accountRowsPerPage->GetValue();
    if (rowsPerPage > 0) {
        entity::Config::SetConfigValue(
            entity::Config::KEY_ACCOUNT_PAGE_SIZE,
            rowsPerPage
        );
    }
    int stake = defaultStake->GetValue();
    if (stake > 0) {
        entity::Config::SetConfigValue("defaultStake", stake);
    }
    int interval = refreshInterval->GetValue();
    if (interval > 0) {
        entity::Config::SetConfigValue(
            entity::Config::KEY_REFRESH_INTERVAL,
            interval
        );
    }
    EndModal(wxID_OK);
}

void Settings::OnCancel(const wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}

}
}
