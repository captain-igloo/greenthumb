/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>

#include "dialog/PaginationToolbar.h"
#include "ArtProvider.h"

namespace greenthumb {
namespace dialog {

wxDEFINE_EVENT(PAGE_CHANGED, wxCommandEvent);

PaginationToolbar::PaginationToolbar(wxWindow* parent, wxWindowID id) : wxToolBar(parent, id) {
    refreshId = wxWindow::NewControlId();
    previousId = wxWindow::NewControlId();
    nextId = wxWindow::NewControlId();

    wxBitmap refreshBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::REFRESH);
    wxBitmap previousBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_PREVIOUS);
    wxBitmap nextBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_NEXT);

    AddTool(refreshId, wxT("Refresh"), refreshBitmap);
    AddTool(previousId, wxT("Previous page"), previousBitmap);
    AddTool(nextId, wxT("Next Page"), nextBitmap);

    EnableTool(previousId, false);
    EnableTool(nextId, false);
    Realize();

    Bind(wxEVT_TOOL, &PaginationToolbar::OnClickNext, this, nextId);
    Bind(wxEVT_TOOL, &PaginationToolbar::OnClickPrevious, this, previousId);
}

const wxWindowID& PaginationToolbar::GetRefreshId() {
    return refreshId;
}

const wxWindowID& PaginationToolbar::GetPreviousId() {
    return previousId;
}

const wxWindowID& PaginationToolbar::GetNextId() {
    return nextId;
}

void PaginationToolbar::SetMoreAvailable(bool moreAvailable) {
    EnableTool(previousId, currentPage > 1);
    EnableTool(nextId, moreAvailable);
}

void PaginationToolbar::OnClickPrevious(const wxCommandEvent& event) {
    if (currentPage > 0) {
        currentPage--;
        QueueEvent(new wxCommandEvent(PAGE_CHANGED));
        EnableTool(previousId, false);
        EnableTool(nextId, false);
    }
}

void PaginationToolbar::OnClickNext(const wxCommandEvent& event) {
    currentPage++;
    QueueEvent(new wxCommandEvent(PAGE_CHANGED));
    EnableTool(previousId, false);
    EnableTool(nextId, false);
}

uint32_t PaginationToolbar::GetCurrentPage() const {
    return currentPage;   
}

}
}