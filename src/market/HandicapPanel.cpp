/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "market/HandicapPanel.h"
#include "ArtProvider.h"
#include "Util.h"

namespace greenthumb {
namespace market {

wxDEFINE_EVENT(HANDICAP_CHANGED, wxCommandEvent);

HandicapPanel::HandicapPanel(wxPanel* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
    const wxString& name) : wxPanel(parent, id, pos, size, style, name) {

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    int borderWidth = 10;
    int borderFlags = wxTOP | wxRIGHT | wxBOTTOM;

    wxSize iconSize = wxSize(36, 36);
    wxWindowID prevButtonId = wxWindow::NewControlId();
    wxButton* prevButton = new wxButton(this, prevButtonId, wxEmptyString, wxDefaultPosition, iconSize, wxBORDER_NONE);
    wxBitmap previousBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_PREVIOUS);
    prevButton->SetBitmap(previousBitmap);
    sizer->Add(prevButton);

    handicapText = new wxStaticText(this, wxID_ANY, "");
    sizer->Add(handicapText, 0, borderFlags, borderWidth);

    wxWindowID nextButtonId = wxWindow::NewControlId();
    wxButton* nextButton = new wxButton(this, nextButtonId, wxEmptyString, wxDefaultPosition, iconSize, wxBORDER_NONE);
    wxBitmap nextBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_NEXT);
    nextButton->SetBitmap(nextBitmap);
    sizer->Add(nextButton);

    Show(false);

    Bind(wxEVT_BUTTON, &HandicapPanel::OnClickPrevious, this, prevButtonId);
    Bind(wxEVT_BUTTON, &HandicapPanel::OnClickNext, this, nextButtonId);
}

void HandicapPanel::AddHandicap(const int64_t selectionId, const double handicap) {
    if (this->selectionId == 0) {
        this->selectionId = selectionId;
    }

    if (selectionId == this->selectionId) {
        runnerHandicaps.insert(handicap * SCALE_FACTOR);
        if (runnerHandicaps.size() == 1) {
            currentHandicap = runnerHandicaps.begin();
            handicapText->SetLabel(DoubleToString((*currentHandicap) / SCALE_FACTOR, 1));
            wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
            QueueEvent(handicapEvent);
        }
    }
}

void HandicapPanel::OnClickPrevious(const wxCommandEvent& event) {
    if (currentHandicap != runnerHandicaps.begin()) {
        --currentHandicap;
        handicapText->SetLabel(DoubleToString(static_cast<double>(*currentHandicap) / SCALE_FACTOR, 1));
        wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
        QueueEvent(handicapEvent);
    }
}

void HandicapPanel::OnClickNext(wxCommandEvent& event) {
    ++currentHandicap;
    if (currentHandicap != runnerHandicaps.end()) {
        handicapText->SetLabel(DoubleToString(static_cast<double>(*currentHandicap) / SCALE_FACTOR, 1));
        wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
        QueueEvent(handicapEvent);
    } else {
        // backtrack, we don't want to past the end of the set.
        --currentHandicap;
    }
}

const double HandicapPanel::GetHandicap(int64_t selectionId) const {
    double runnerHandicap = static_cast<double>(*currentHandicap) / SCALE_FACTOR;

    if (selectionId == this->selectionId) {
        return runnerHandicap;
    }

    return runnerHandicap * -1;
}

}
}
