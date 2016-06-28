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

        const static uint32_t PAGE_SIZE;

        uint32_t currentPage;
        uint32_t numberPages;
        wxGrid* grid;

        virtual void Render() = 0;
        void UpdateToolbar();

    private:

        wxWindowID refreshButtonId;
        wxWindowID firstPageButtonId;
        wxWindowID previousPageButtonId;
        wxWindowID nextPageButtonId;
        wxWindowID lastPageButtonId;
        wxToolBar* toolbar;
        worker::WorkerManager workerManager;

        void OnClickRefresh(wxCommandEvent& event);
        void OnClickFirst(wxCommandEvent& event);
        void OnClickPrevious(wxCommandEvent& event);
        void OnClickNext(wxCommandEvent& event);
        void OnClickLast(wxCommandEvent& event);

};

}

#endif // PAGINATEDGRID_H
