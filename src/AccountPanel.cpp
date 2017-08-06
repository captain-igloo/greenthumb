/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/button.h>

#include "AccountPanel.h"
#include "AccountStatementFull.h"
#include "AccountStatementSummary.h"

#include "entity/AccountStatementItem.h"
#include "worker/GetAccountStatement.h"

namespace greenthumb {

AccountPanel::AccountPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxNotebook(parent, id, pos, size, style, name) {

    accountStatementFull = new AccountStatementFull(this);
    accountStatementSummary = new AccountStatementSummary(this);
    currentOrdersGrid = new CurrentOrdersGrid(this);

    AddPage(accountStatementFull, "Full");
    AddPage(accountStatementSummary, "Summary");
    AddPage(currentOrdersGrid, "Current Orders");

    Bind(worker::GET_ACCOUNT_STATEMENT, &AccountPanel::OnGetAccountStatement, this, wxID_ANY);
}

void AccountPanel::SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets) {
    currentOrdersGrid->SetBetfairMarketsCache(betfairMarkets);
}

void AccountPanel::OnGetAccountStatement(const wxThreadEvent& event) {
    accountStatementFull->OnGetAccountStatement();
    accountStatementSummary->OnGetAccountStatement();
}

}
