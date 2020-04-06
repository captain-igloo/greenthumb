/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WIDGET_MARKET_MULTIWINNERRUNNERPRICES_H
#define WIDGET_MARKET_MULTIWINNERRUNNERPRICES_H

#include <wx/wx.h>
#include <wx/stattext.h>

#include "RunnerPrices.h"

namespace greenthumb {
namespace widget {
namespace market {

class MultiWinnerRunnerPrices : public RunnerPrices {
    public:
        MultiWinnerRunnerPrices(wxWindow* parent);

        virtual void SetPendingPlaceInstruction(const greentop::sport::PlaceInstruction& placeInstruction);

    private:
        wxStaticText* profitIfWinText = NULL;
        wxStaticText* profitSeparator = NULL;
        wxStaticText* profitIfLoseText = NULL;
        wxStaticText* pendingIfWinText = NULL;
        wxStaticText* pendingSeparatorText = NULL;
        wxStaticText* pendingIfLoseText = NULL;
        wxSizer* sizer = NULL;
        double profitAndLossIfWin = 0;
        double profitAndLossIfLose = 0;

        virtual void UpdateProfitAndLossIfWin();

        void SetProfitLabel(wxStaticText* profitText, double profit) const;
};

}
}
}

#endif // WIDGET_MARKET_MULTIWINNERRUNNERPRICES_H
