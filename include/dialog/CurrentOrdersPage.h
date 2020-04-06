/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_CURRENTORDERSPAGE_H
#define DIALOG_CURRENTORDERSPAGE_H

#include <wx/wx.h>
#include <wx/panel.h>

#include "dialog/PaginationToolbar.h"
#include "entity/Market.h"
#include "worker/WorkerManager.h"
#include "CurrentOrder.h"

namespace greenthumb {
namespace dialog {

/**
 * Displays page of (un)matched orders and a pagination toolbar.
 */
class CurrentOrdersPage : public wxPanel {
    public:
        /**
         * Constructor.
         *
         * @param parent The parent window.
         */
        CurrentOrdersPage(wxWindow* parent);

       /**
        * Sets the market whose orders will be displayed.
        *
        * @param market The market to set.
        */
        void SetMarket(const entity::Market& market);

        /**
         * Request current orders from betfair. 
         */
        void RefreshOrders();

    protected:
        /** Message displayed when there are no orders. */
        wxStaticText* noOrdersMessage = NULL;

        /**
         * Creates and returns panel that displays a matched or unmatched order.
         *
         * @return A current order to display.
         */
        virtual CurrentOrder* CreateOrder() = 0;

        /**
         * Returns the OrderProjection, either EXECUTION_COMPLETE or EXECUTABLE.
         *
         * @return The OrderProjection for this page.
         */
        virtual greentop::sport::OrderProjection GetOrderProjection() const = 0;
    private:
        /** Manages worker threads. */
        worker::WorkerManager workerManager;
        /** The orders' market. */
        entity::Market market;
        /** Refresh, next page, previous page buttons.*/
        PaginationToolbar* toolbar;
        
        /**
         * Calls RefreshOrders().
         *
         * @param event The click event.
         */
        void OnClickRefresh(const wxCommandEvent& event);

        /**
         * Calls RefreshOrders().
         *
         * @param event The page changed event.
         */
        void OnPageChanged(const wxCommandEvent& event);

        /**
         * Synchronise orders with listCurrentOrders response from betfair.
         *
         * @param event The worker event.
         */
        void OnListCurrentOrders(const wxThreadEvent& event);
};

}
}

#endif // DIALOG_CURRENTORDERSPAGE_H
