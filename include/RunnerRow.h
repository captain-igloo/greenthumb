#ifndef RUNNERROW_H
#define RUNNERROW_H

#include <greentop/betting/MarketBook.h>
#include <greentop/betting/Runner.h>
#include <greentop/betting/PlaceInstruction.h>
#include <greentop/enum/Side.h>

#include <wx/button.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#include "entity/Market.h"

#include "PriceButton.h"

namespace greenthumb {

class RunnerRow {

    public:
        RunnerRow();

        RunnerRow(wxWindow* parent);

        void SetRunner(const entity::Market& market, const greentop::MarketBook& marketBook, const greentop::Runner& r);
        void SetProfit(double profit);
        void RefreshPrices();
        uint64_t GetSelectionId();
        void SetPendingPlaceInstruction(const greentop::PlaceInstruction& placeInstruction);

    protected:
    private:

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
        greentop::Runner runner;
        wxWindowID chartButtonId;
        std::string currencySymbol;

        void SetButtonLabel(PriceButton* button, const double price, const double size, const wxString& currencySymbol) const;
        void ResetButton(PriceButton* button);
        void OnClickChart(wxEvent& event);
        PriceButton* CreateButton(wxWindow* parent, const greentop::Side& side, const wxColour& colour);
};

}

#endif // RUNNERROW_H

