/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#ifndef RUNNERROW_H
#define RUNNERROW_H

#include <greentop/sport/MarketBook.h>
#include <greentop/sport/Runner.h>
#include <greentop/sport/PlaceInstruction.h>
#include <greentop/sport/enum/Side.h>

#include <wx/wx.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#include "entity/Market.h"

#include "PriceButton.h"

namespace greenthumb {

/**
 * Displays prices for a runner.
 */
class RunnerRow {

    public:

        /**
         * Constructor.
         *
         * @param parent The parent window.
         */
        RunnerRow(wxWindow* parent);

        /**
         * Sets the market catalogue, prices and runner information from betfair.
         *
         * @param market The market catalogue.
         * @param marketBook The market prices.
         * @param runner The runner.
         */
        void SetRunner(const entity::Market& market, const greentop::MarketBook& marketBook,
            const greentop::Runner& runner);

        /**
         * Sets the profit / loss for this runner.
         *
         * @param profit The profit / loss.
         */
        void SetProfit(double profit);

        /**
         * Update the prices display.
         */
        void RefreshPrices();

        /**
         * Returns the runner's id.
         *
         * @return The runner's id.
         */
        const int64_t GetSelectionId() const;

        /**
         * Sets the potential profit / loss for a bet that hasn't been placed yet.
         *
         * @param placeInstruction The pending place instruction.
         */
        void SetPendingPlaceInstruction(const greentop::PlaceInstruction& placeInstruction);

        /**
         * Sets the handicap.
         *
         * @param handicap The handicap.
         */
        void SetHandicap(const double handicap);

        /**
         * Returns the name of runner, including handicap if applicable.
         *
         * @return The name of the runner including handicap.
         */
        const wxString GetRunnerName() const;

    private:
        const static unsigned scaleFactor = 100;
        const static int chartButtonWidth = 30;
        const static int priceButtonWidth = 60;

        wxStaticText* runnerName;
        wxStaticText* profitAndLossIfWinText;
        double lastPriceTraded;
        double profitAndLossIfWin;
        wxStaticText* pendingProfit;
        PriceButton* bestBackPrice1;
        PriceButton* bestBackPrice2;
        PriceButton* bestBackPrice3;
        PriceButton* bestLayPrice1;
        PriceButton* bestLayPrice2;
        PriceButton* bestLayPrice3;
        entity::Market market;
        greentop::MarketBook marketBook;
        // greentop::Runner runner;
        int64_t handicap = 0;
        std::map<int64_t, greentop::Runner> runners;
        wxWindowID chartButtonId;
        std::string currencySymbol;
        int64_t selectionId = 0;

        /**
         * Sets a price button label.
         *
         * @param button The price button.
         * @param price The new price.
         * @param size The new size.
         * @param currencySymbol The currency symbol.
         */
        void SetButtonLabel(PriceButton* button, const double price, const double size, const wxString& currencySymbol) const;

        /**
         * Sets the button's label to an empty string and set its price to the default.
         *
         * @param button The button to reset.
         */
        void ResetButton(PriceButton* button) const;

        /**
         * Show the price history dialog.
         *
         * @param event The click event.
         */
        void OnClickChart(wxEvent& event);

        /**
         * Creates a price button.
         *
         * @param parent The parent window.
         * @param side Either back or lay.
         * @param colour The colour of the button.
         * @return The price button.
         */
        PriceButton* CreateButton(wxWindow* parent, const greentop::Side& side, const wxColour& colour) const;

        /**
         * Gets the current runner, ie the runner associated with the currently selected handicap.
         *
         * @param The current runner.
         */
        const greentop::Runner& GetRunner();

        /**
         * Updates the runner name label.
         */
        void UpdateRunnerName();
};

}

#endif // RUNNERROW_H

