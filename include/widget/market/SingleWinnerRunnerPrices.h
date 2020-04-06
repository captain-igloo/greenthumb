/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WIDGET_MARKET_SINGLEWINNERRUNNERPRICES_H
#define WIDGET_MARKET_SINGLEWINNERRUNNERPRICES_H

#include "RunnerPrices.h"

namespace greenthumb {
namespace widget {
namespace market {

class SingleWinnerRunnerPrices : public RunnerPrices {
    public:
        SingleWinnerRunnerPrices(wxWindow* parent);

        virtual void SetPendingPlaceInstruction(const greentop::sport::PlaceInstruction& placeInstruction);

    private:
        wxStaticText* profitAndLossIfWinText = NULL;
        wxStaticText* pendingProfit = NULL;
        double profitAndLossIfWin;

        virtual void UpdateProfitAndLossIfWin();
};

}
}
}

#endif // WIDGET_MARKET_SINGLEWINNERRUNNERPRICES_H
