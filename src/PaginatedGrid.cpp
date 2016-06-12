#include <wx/sizer.h>

#include "ArtProvider.h"
#include "PaginatedGrid.h"

#include "worker/GetAccountStatement.h"

namespace greenthumb {

const uint32_t PaginatedGrid::PAGE_SIZE = 20;

PaginatedGrid::PaginatedGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos,
    const wxSize& size, long style, const wxString& name) :
    wxPanel(parent, id, pos, size, style, name), workerManager(this) {

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    refreshButtonId = wxWindow::NewControlId();
    firstPageButtonId = wxWindow::NewControlId();
    previousPageButtonId = wxWindow::NewControlId();
    nextPageButtonId = wxWindow::NewControlId();
    lastPageButtonId = wxWindow::NewControlId();

    wxBitmap refreshBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::REFRESH);

    wxBitmap resultsetFirstBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_FIRST);
    wxBitmap resultsetPreviousBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_PREVIOUS);
    wxBitmap resultsetNextBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_NEXT);
    wxBitmap resultsetLastBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_LAST);

    toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition);
    sizer->Add(toolbar, 0, wxEXPAND);
    toolbar->AddTool(refreshButtonId, wxT("Refresh"), refreshBitmap);
    toolbar->AddTool(firstPageButtonId, wxT("First page"), resultsetFirstBitmap);
    toolbar->AddTool(previousPageButtonId, wxT("Previous page"), resultsetPreviousBitmap);
    toolbar->AddTool(nextPageButtonId, wxT("Next Page"), resultsetNextBitmap);
    toolbar->AddTool(lastPageButtonId, wxT("Last page"), resultsetLastBitmap);
    toolbar->Realize();

    currentPage = 1;

    Bind(wxEVT_TOOL, &PaginatedGrid::OnClickRefresh, this, refreshButtonId);
    Bind(wxEVT_TOOL, &PaginatedGrid::OnClickFirst, this, firstPageButtonId);
    Bind(wxEVT_TOOL, &PaginatedGrid::OnClickPrevious, this, previousPageButtonId);
    Bind(wxEVT_TOOL, &PaginatedGrid::OnClickNext, this, nextPageButtonId);
    Bind(wxEVT_TOOL, &PaginatedGrid::OnClickLast, this, lastPageButtonId);

    workerManager.Bind(worker::GET_ACCOUNT_STATEMENT);
}

void PaginatedGrid::OnClickRefresh(wxCommandEvent& event) {

    workerManager.RunWorker(new worker::GetAccountStatement(&workerManager));

}

void PaginatedGrid::OnClickFirst(wxCommandEvent& event) {
    if (currentPage != 1) {
        currentPage = 1;
        Render();
    }
}

void PaginatedGrid::OnClickPrevious(wxCommandEvent& event) {
    if (currentPage > 1) {
        --currentPage;
        Render();
    }
}

void PaginatedGrid::OnClickNext(wxCommandEvent& event) {
    if (currentPage < numberPages) {
        ++currentPage;
        Render();
    }
}

void PaginatedGrid::OnClickLast(wxCommandEvent& event) {
    if (currentPage != numberPages) {
        currentPage = numberPages;
        Render();
    }
}

void PaginatedGrid::UpdateToolbar() {

    if (currentPage == 1) {
        toolbar->EnableTool(firstPageButtonId, false);
        toolbar->EnableTool(previousPageButtonId, false);
    } else {
        toolbar->EnableTool(firstPageButtonId, true);
        toolbar->EnableTool(previousPageButtonId, true);
    }

    if (currentPage == numberPages) {
        toolbar->EnableTool(nextPageButtonId, false);
        toolbar->EnableTool(lastPageButtonId, false);
    } else {
        toolbar->EnableTool(nextPageButtonId, true);
        toolbar->EnableTool(lastPageButtonId, true);
    }

}


}
