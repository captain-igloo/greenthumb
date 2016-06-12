#ifndef MARKETPANEL_H
#define MARKETPANEL_H

#include <greentop/betting/MarketBook.h>
#include <greentop/betting/ListMarketBookRequest.h>
#include <greentop/betting/ListMarketBookResponse.h>

#include <wx/panel.h>
#include <wx/timer.h>
#include <wx/toolbar.h>

#include "dialog/CurrentOrders.h"

#include "entity/Market.h"

#include "worker/ListMarketBook.h"
#include "worker/ListMarketProfitAndLoss.h"
#include "worker/WorkerManager.h"

#include "CurrentOrder.h"
#include "MarketPanels.h"
#include "RunnerRow.h"

namespace greenthumb {

class MarketPanels;

class MarketPanel : public wxPanel {

    public:

        MarketPanel(MarketPanels* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, long style = wxSUNKEN_BORDER);

        void SetMarket(const greentop::menu::Node& node);
        void SetMarket(const entity::Market& market);

    private:

        wxPanel* leftPanel;
        wxPanel* rightPanel;
        entity::Market market;
        std::map<int, RunnerRow*> runnerRows;
        MarketPanels* marketPanels;
        wxStaticText* marketName;
        greentop::ListMarketBookRequest listMarketBookRequest;
        greentop::ListMarketBookResponse listMarketBookResponse;
        wxTimer refreshTimer;
        worker::WorkerManager workerManager;
        wxToolBar* toolbar;
        int inPlayToolId;
        wxStaticText* marketStatus;
        dialog::CurrentOrders* currentOrdersDialog;
        wxToolBarToolBase* currentOrdersTool;
        std::string fullMarketName;
        std::string marketId;
        greentop::MarketBook marketBook;

        MarketPanel(const MarketPanel& marketPanel);

        // MarketPanel& operator= (const MarketPanel& marketPanel);

        void OnClick(const wxCommandEvent& event);
        void OnPlaceBet(const wxThreadEvent& event);
        void OnCancelOrders(const wxThreadEvent& event);
        void RefreshPrices();
        void RefreshPrices(wxEvent& event);
        void OnMarketUpdated(const wxThreadEvent& event);
        void OnClickClose(const wxCommandEvent& event);
        void SyncRunnerRows();
        void OnSize(const wxSizeEvent& event);
        void OnListMarketProfitAndLoss(const wxThreadEvent& event);
        void OnPlaceOrderPending(const wxCommandEvent& event);
        void UpdateToolBar();
        void UpdateMarketStatus();
        void ShowCurrentOrders(const wxEvent& event);
        void OnListCurrentOrders(wxThreadEvent& event);

};

}

#endif // MARKETPANEL_H
