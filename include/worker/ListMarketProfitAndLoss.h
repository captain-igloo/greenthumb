#ifndef WORKER_LISTMARKETPROFITANDLOSS_H
#define WORKER_LISTMARKETPROFITANDLOSS_H

#include <greentop/ExchangeApi.h>
#include <string>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(LIST_MARKET_PROFIT_AND_LOSS, wxThreadEvent);

class ListMarketProfitAndLoss : public Worker {
    public:

        ListMarketProfitAndLoss(wxEvtHandler* eventHandler,
            const greentop::Exchange exchange, const  std::string& marketId);
    protected:
        virtual ExitCode Entry();
    private:

        greentop::Exchange exchange;
        std::string marketId;
        greentop::ListMarketProfitAndLossResponse DoListMarketProfitAndLoss();
};

}
}

#endif // WORKER_LISTMARKETPROFITANDLOSS_H
