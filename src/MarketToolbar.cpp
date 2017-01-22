/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */

#include "ArtProvider.h"
#include "ImagePanel.h"
#include "MarketToolbar.h"

namespace greenthumb {

MarketToolbar::MarketToolbar(wxWindow* parent, const wxWindowID id,
    const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxPanel(parent, id, pos, size, style, name) {

    int borderWidth = 10;
    int borderFlags = wxTOP | wxRIGHT | wxBOTTOM;

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    inPlayButton = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(16, 16), wxBORDER_NONE);
    inPlayButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::BLANK));
    sizer->Add(inPlayButton, 0, wxLEFT | borderFlags, borderWidth);

    marketName = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(marketName, 0, borderFlags, borderWidth);

    marketStatus = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(marketStatus, 1, wxEXPAND | borderFlags, borderWidth);

    currentOrdersButtonId = wxWindow::NewControlId();
    currentOrdersButton = new wxButton(this, currentOrdersButtonId, wxEmptyString, wxDefaultPosition, wxSize(16, 16), wxBORDER_NONE);
    currentOrdersButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::VIEW_LIST));
    sizer->Add(currentOrdersButton, 0, borderFlags, borderWidth);

    refreshButtonId = wxWindow::NewControlId();
    wxButton* refreshButton = new wxButton(this, refreshButtonId, wxEmptyString, wxDefaultPosition, wxSize(16, 16), wxBORDER_NONE);
    refreshButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::REFRESH));
    sizer->Add(refreshButton, 0, borderFlags, borderWidth);

    closeButtonId = wxWindow::NewControlId();
    wxButton* closeButton = new wxButton(this, closeButtonId, wxEmptyString, wxDefaultPosition, wxSize(16, 16), wxBORDER_NONE);
    closeButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::CLOSE));
    sizer->Add(closeButton, 0, borderFlags, borderWidth);

    SetSizer(sizer);

}

void MarketToolbar::SetMarketName(const wxString& marketName) {
    this->marketName->SetLabel(marketName);
}

void MarketToolbar::SetMarketStatus(const wxString& marketStatus) {
    this->marketStatus->SetLabel(marketStatus);
}

const wxWindowID MarketToolbar::GetCloseButtonId() {
    return closeButtonId;
}

const wxWindowID MarketToolbar::GetRefreshButtonId() {
    return refreshButtonId;
}

const wxWindowID MarketToolbar::GetCurrentOrdersButtonId() {
    return currentOrdersButtonId;
}

void MarketToolbar::SetInPlay(bool inPlay) {
    if (inPlay) {
        inPlayButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::TICK_GREEN));
    } else {
        inPlayButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::TICK_GREY));
    }
}

}
