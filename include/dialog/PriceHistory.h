/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_PRICEHISTORY_H
#define DIALOG_PRICEHISTORY_H

#include <greentop/sport/Runner.h>

#include <wx/dialog.h>
#include <wx/panel.h>

#include "entity/Market.h"
#include "ImagePanel.h"

namespace greenthumb {
namespace dialog {

/**
 * Show a chart of the matched price of a runner over time.
 */
class PriceHistory : public wxDialog {
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
         *               user to set Motif resource values for individual dialog boxes.
         */
        PriceHistory(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, const wxString& name = wxDialogNameStr);

        /**
         * Sets the market and runner whose price history will be shown.
         *
         * @param market The market.
         * @param runner The runner.
         */
        void SetRunner(const entity::Market& market, const greentop::sport::Runner& runner);

        /**
         * Sets the last price traded.
         *
         * @param lastPriceTraded The last price traded.
         */
        void SetLastPriceTraded(const double lastPriceTraded);

    protected:
    private:
        static const int CHART_WIDTH = 350;
        static const int CHART_HEIGHT = 255;

        wxStaticText* lastPriceTraded = NULL;
        ImagePanel* chartPanel = NULL;
        wxStaticText* bettingOn = NULL;

        void LoadChart(const entity::Market& market, const greentop::sport::Runner& runner);
        void OnClose(wxCommandEvent& event);
};

}
}

#endif // DIALOG_PRICEHISTORY_H
