/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "dialog/CurrentOrdersDialog.h"
#include "dialog/PaginationToolbar.h"
#include "ArtProvider.h"

namespace greenthumb {
namespace dialog {

CurrentOrdersDialog::CurrentOrdersDialog(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name) {

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    SetSizer(vbox);
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);
    vbox->Add(notebook, 0, wxEXPAND);

    unmatchedOrders = new UnmatchedOrders(notebook);
    notebook->AddPage(unmatchedOrders, "Unmatched");

    matchedOrders = new MatchedOrders(notebook);
    notebook->AddPage(matchedOrders, "Matched");

    wxSizer* buttonSizer = CreateButtonSizer(wxOK);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxALIGN_RIGHT);
    }

    Fit();
}

void CurrentOrdersDialog::RefreshOrders() {
    unmatchedOrders->RefreshOrders();
    matchedOrders->RefreshOrders();
}

void CurrentOrdersDialog::SetMarket(const entity::Market& market) {
    unmatchedOrders->SetMarket(market);
    matchedOrders->SetMarket(market);
}

}
}