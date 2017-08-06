/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#ifndef PAGINATEDGRID_H
#define PAGINATEDGRID_H

#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/toolbar.h>
#include "worker/WorkerManager.h"

namespace greenthumb {

class PaginatedGrid : public wxPanel {
    public:
        PaginatedGrid(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name=wxPanelNameStr);

    protected:
        uint32_t currentPage;
        uint32_t numberPages;
        wxGrid* grid;
        worker::WorkerManager workerManager;

        virtual void Render() = 0;
        void UpdateToolbar();
        virtual void OnClickRefresh(wxCommandEvent& event);
        virtual void OnClickFirst(wxCommandEvent& event);
        virtual void OnClickPrevious(wxCommandEvent& event);
        virtual void OnClickNext(wxCommandEvent& event);
        virtual void OnClickLast(wxCommandEvent& event);

    private:

        wxWindowID refreshButtonId;
        wxWindowID firstPageButtonId;
        wxWindowID previousPageButtonId;
        wxWindowID nextPageButtonId;
        wxWindowID lastPageButtonId;
        wxToolBar* toolbar;
};

}

#endif // PAGINATEDGRID_H
