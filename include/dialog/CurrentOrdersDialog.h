/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_CURRENTORDERSDIALOG_H
#define DIALOG_CURRENTORDERSDIALOG_H

#include <wx/wx.h>
#include <wx/dialog.h>

#include "dialog/UnmatchedOrders.h"
#include "dialog/MatchedOrders.h"
#include "entity/Market.h"

namespace greenthumb {
namespace dialog {

/**
 * Current orders dialog.  Displays a list of unmatched orders for a market.
 */
class CurrentOrdersDialog : public wxDialog {
    public:

        /**
        * Constructor.
        *
        * @param parent Pointer to the parent window.
        * @param id The identifier of the dialog.
        * @param title The title of the dialog.
        * @param pos The dialog position.
        * @param size The dialog size.
        * @param style The window style.
        * @param name The dialog name.
        */
        CurrentOrdersDialog(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize,
            long style=wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, const wxString& name=wxDialogNameStr);

        /**
        * Sets the market whose orders will be displayed.
        *
        * @param market The market to set.
        */
        void SetMarket(const entity::Market& market);

        void RefreshOrders();

    private:
        /** The orders' market. */
        entity::Market market;
        /** The unmatched orders page. */
        UnmatchedOrders* unmatchedOrders = NULL;
        /** The matched orders page. */
        MatchedOrders* matchedOrders = NULL;
};


}
}

#endif // DIALOG_CURRENTORDERSDIALOG_H
