#ifndef ACCOUNTPANEL_H
#define ACCOUNTPANEL_H

#include <wx/grid.h>
#include <wx/notebook.h>

#include "AccountStatementFull.h"
#include "AccountStatementSummary.h"

namespace greenthumb {

class AccountPanel : public wxNotebook {
    public:
        AccountPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxNB_BOTTOM, const wxString& name=wxNotebookNameStr);
    protected:
    private:

        AccountStatementFull* accountStatementFull;
        AccountStatementSummary* accountStatementSummary;

        void OnGetAccountStatement(wxThreadEvent& event);
};

}

#endif // ACCOUNTPANEL_H
