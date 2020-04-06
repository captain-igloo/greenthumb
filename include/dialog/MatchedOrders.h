/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_MATCHEDORDERS_H
#define DIALOG_MATCHEDORDERS_H

#include "dialog/CurrentOrdersPage.h"

namespace greenthumb {
namespace dialog {

/**
 * Displays a page of matched orders and a pagination toolbar.
 */
class MatchedOrders: public CurrentOrdersPage {
    public:
        /**
         * Constructor.
         *
         * @param parent The parent window.
         */
        MatchedOrders(wxWindow* parent);

    protected:
        /**
         * Creates and returns panel that displays a matched order.
         *
         * @return A matched order to display.
         */
        CurrentOrder* CreateOrder();

        /**
         * Returns the order projection.
         *
         * @return The EXECUTION_COMPLETE order projection.
         */
        greentop::sport::OrderProjection GetOrderProjection() const;
};

}
}

#endif // DIALOG_MATCHEDORDERS_H
