/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef CURRENTORDER_H
#define CURRENTORDER_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <greentop/sport/CurrentOrderSummary.h>

#include "entity/Market.h"
#include "worker/WorkerManager.h"
#include "OddsSpinCtrl.h"
#include "StakeSpinCtrl.h"

namespace greenthumb {

/**
 * Displays an order (either matched or unmatched).
 */
class CurrentOrder : public wxPanel {

    public:

        /**
         * Constructor.
         *
         * @param parent The parent window.
         * @param id An identifier for the panel.
         * @param pos The panel position.
         * @param size The panel size.
         * @param style The window style.
         * @param name Window name.
         */
        CurrentOrder(wxWindow* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxSUNKEN_BORDER,
            const wxString& name = wxPanelNameStr);

        /**
         * Populate the order's fields - odds, stake, selection etc.
         *
         * @param currentOrderSummary The order, received from betfair.
         */
        virtual void SetCurrentOrderSummary(const greentop::sport::CurrentOrderSummary& currentOrderSummary) = 0;

        /** 
         * Gets the betfair CurrentOrderSummary.
         */
        greentop::sport::CurrentOrderSummary GetCurrentOrderSummary();

        /**
         * Set the order's market.
         *
         * @param market The order's market.
         */
        void SetMarket(const entity::Market& market);

    protected:
        /** The order's market. */
        entity::Market market;
        /** The order. */
        greentop::sport::CurrentOrderSummary currentOrderSummary;

};

}

#endif // CURRENTORDER_H
