#ifndef ACCOUNTSTATEMENTSUMMARY_H
#define ACCOUNTSTATEMENTSUMMARY_H

#include "PaginatedGrid.h"

namespace greenthumb {

class AccountStatementSummary : public PaginatedGrid {
    public:
        AccountStatementSummary(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name=wxPanelNameStr);

        void OnGetAccountStatement();

    protected:

        void Render();

    private:

        void GetNumberPages();

};

}

#endif // ACCOUNTSTATEMENTSUMMARY_H
