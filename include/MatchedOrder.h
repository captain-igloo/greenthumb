/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#ifndef MATCHEDORDER_H
#define MATCHEDORDER_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#include <greentop/sport/CurrentOrderSummary.h>

#include "CurrentOrder.h"

namespace greenthumb {

/**
 * Displays a matched order.
 */
class MatchedOrder : public CurrentOrder {

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
        MatchedOrder(wxWindow* parent, const wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxSUNKEN_BORDER, const wxString& name = wxPanelNameStr);

        /**
         * Populate the order's fields - odds, stake, selection etc.
         *
         * @param currentOrderSummary The order, received from betfair.
         */
        void SetCurrentOrderSummary(const greentop::CurrentOrderSummary& currentOrderSummary);

    protected:
    private:
        /** Displays the runner name. */
        wxStaticText* runnerName = NULL;
        /** Displays the order's odds. */
        wxStaticText* price = NULL;
        /** Displays the order's stake. */
        wxStaticText* stake = NULL;
        /** Displays either "profit" or "liability" */
        wxStaticText* profitOrLiabilityLabel = NULL;
        /** Displays the profit or liability. */
        wxStaticText* profitOrLiability = NULL;

        /**
         * Calculate the profit (for back bets) or liability (for lay bets).
         */
        void UpdateProfitOrLiability();

        DECLARE_NO_COPY_CLASS(MatchedOrder)
};

}

#endif // MATCHEDORDER_H
