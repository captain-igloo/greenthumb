/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef ACCOUNTPANEL_H
#define ACCOUNTPANEL_H

#include <greentop/LRUCache.h>

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/notebook.h>

#include "AccountStatementFull.h"
#include "AccountStatementSummary.h"
#include "CurrentOrdersGrid.h"

namespace greenthumb {

/**
 * Display the account statement.
 */
class AccountPanel : public wxNotebook {
    public:

        /**
        * Constructor.
        *
        * @param parent The parent window.
        * @param id The window identifier.
        * @param pos The window position.
        * @param size The window size.
        * @param style The window style.
        * @param name The name of the control.
        */
        AccountPanel(wxWindow* parent, wxWindowID id);

        void SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets);

    protected:
    private:

        /** The full account statement.  Displays details of each bet. */
        AccountStatementFull* accountStatementFull;
        /** The account statement summary.  Displays a summary of bets grouped by event. */
        AccountStatementSummary* accountStatementSummary;

        CurrentOrdersGrid* currentOrdersGrid;

        /**
         * Render the account statement pages.
         *
         * @param event The worker event.
         */
        void OnGetAccountStatement(const wxThreadEvent& event);

        DECLARE_NO_COPY_CLASS(AccountPanel)
};

}

#endif // ACCOUNTPANEL_H
