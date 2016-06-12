/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_CURRENTORDERS_H
#define DIALOG_CURRENTORDERS_H

#include <wx/dialog.h>

#include "entity/Market.h"
#include "worker/WorkerManager.h"

namespace greenthumb {
namespace dialog {

/**
 * Current orders dialog.  Displays a list of unmatched orders for a market.
 */
class CurrentOrders : public wxDialog {
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
        * @param name Used to associate a name with the window, allowing the application
        *                user to set Motif resource values for individual dialog boxes.
        */
        CurrentOrders(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize,
            long style=wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, const wxString& name=wxDialogNameStr);

        /**
        * Request an update from betfair.
        */
        void Refresh();

        /**
        * Sets the market whose orders will be displayed.
        *
        * @param market The market to set.
        */
        void SetMarket(const entity::Market& market);

        /**
        * Returns the number of unmatched orders.
        *
        * @return The number of orders.
        */
        uint64_t GetNumberOrders();

    protected:
    private:

        /** The orders' market. */
        entity::Market market;
        /** Manages worker threads. */
        worker::WorkerManager workerManager;

        /**
        * Synchronise orders with listCurrentOrders response from betfair.
        *
        * @param event The worker event.
        */
        void OnListCurrentOrders(wxThreadEvent& event);

        /**
        * An order has been modified, so refresh needed.
        *
        * @param event The worker event.
        */
        void OnReplaceOrders(wxThreadEvent& event);

};

}
}

#endif // DIALOG_CURRENTORDERS_H
