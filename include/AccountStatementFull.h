/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
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
        void GetNumberPages();
};

}

#endif // ACCOUNTSTATEMENTFULL_H
