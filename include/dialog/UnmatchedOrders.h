/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_UNMATCHEDORDERS_H
#define DIALOG_UNMATCHEDORDERS_H

#include "dialog/CurrentOrdersPage.h"

namespace greenthumb {
namespace dialog {

/**
 * Displays a page of unmatched orders and a pagination toolbar.
 */
class UnmatchedOrders: public CurrentOrdersPage {
    public:
        /**
         * Constructor.
         *
         * @param parent The parent window.
         */
        UnmatchedOrders(wxWindow* parent);

    protected:
        /**
         * Creates and returns panel that displays an unmatched order.
         *
         * @return A matched order to display.
         */
        CurrentOrder* CreateOrder();

        /**
         * Returns the order projection.
         *
         * @return The EXECUTABLE order projection.
         */
        greentop::OrderProjection GetOrderProjection() const;

        /**
         * An order has been cancelled so remove it.
         *
         * @param event The worker event. 
         */
        void OnCancelOrders(wxThreadEvent& event);

        /**
         * An order has been replaced so call RefreshOrders().
         */
        void OnReplaceOrders(wxThreadEvent& event);
};

}
}

#endif // DIALOG_UNMATCHEDORDERS_H
