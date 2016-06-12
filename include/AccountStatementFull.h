#ifndef ACCOUNTSTATEMENTFULL_H
#define ACCOUNTSTATEMENTFULL_H

#include "PaginatedGrid.h"

namespace greenthumb {

class AccountStatementFull : public PaginatedGrid {
    public:
        AccountStatementFull(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name=wxPanelNameStr);

        void OnGetAccountStatement();

    protected:

        virtual void Render();

    private:

        // const static uint32_t PAGE_SIZE;

        void GetNumberPages();
};

}

#endif // ACCOUNTSTATEMENTFULL_H
