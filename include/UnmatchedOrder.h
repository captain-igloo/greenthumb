/**
* Copyright 2020 Colin Doig.  Distributed under the MIT license.
*/
#ifndef UNMATCHEDORDER_H
#define UNMATCHEDORDER_H

#include <wx/wx.h>
#include <wx/button.h>

#include <greentop/sport/CurrentOrderSummary.h>

#include "CurrentOrder.h"

namespace greenthumb {

/**
 * Displays an unmatched order.
 */
class UnmatchedOrder : public CurrentOrder {

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
        UnmatchedOrder(wxWindow* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL | wxSUNKEN_BORDER,
            const wxString& name = wxPanelNameStr);

        /**
         * Populate the order's fields - odds, stake, selection etc.
         *
         * @param currentOrderSummary The order, received from betfair.
         */
        void SetCurrentOrderSummary(const greentop::sport::CurrentOrderSummary& currentOrderSummary);

    protected:
    private:
        const static uint32_t CELL_WIDTH = 65;

        /** Displays the runner name */
        wxStaticText* runnerName = NULL;
        /** Displays the order's odds. */
        OddsSpinCtrl* oddsSpin = NULL;
        /** Displays the order's stake. */
        wxStaticText* stake = NULL;
        /** Displays either "profit" or "liability" */
        wxStaticText* profitOrLiabilityLabel = NULL;
        /** Displays the profit or liability. */
        wxStaticText* profitOrLiability = NULL;
        /** Button to submit changes to the bet. */
        wxButton* submitButton = NULL;
        /** Manages replace / cancel orders workers. */
        worker::WorkerManager workerManager;

        /**
         * Update profit / liability when the odds are changed.
         *
         * @param event The spin event.
         */
        void OnSpin(wxSpinDoubleEvent& event);

        /**
         * Attempt to cancel the order.
         *
         * @param event The click event.
         */
        void OnClickCancel(wxCommandEvent& event);

        /**
         * Submit the new odds to betfair.
         *
         * @param event The click event.
         */
        void OnClickSubmit(wxCommandEvent& event);

        /**
         * Re-calculate profit / liability.
         */
        void UpdateProfitOrLiability();

        DECLARE_NO_COPY_CLASS(UnmatchedOrder)
};

}

#endif // UNMATCHEDORDER_H
