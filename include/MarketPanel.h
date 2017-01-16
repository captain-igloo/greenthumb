/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#ifndef MARKETPANEL_H
#define MARKETPANEL_H

#include <greentop/sport/MarketBook.h>
#include <greentop/sport/ListMarketBookRequest.h>
#include <greentop/sport/ListMarketBookResponse.h>

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

/**
 * Display market prices.
 */
class MarketPanel : public wxPanel {

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
        MarketPanel(MarketPanels* parent, const wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxSUNKEN_BORDER, const wxString& name = wxPanelNameStr);

        /**
         * Update the market name.
         *
         * @param node The market menu node.
         */
        void SetMarket(const greentop::menu::Node& node);

        /**
         * Add runners and refresh prices.
         *
         * @param market The betfair market catalogue.
         */
        void SetMarket(const entity::Market& market);

    private:
        /** Display runners and their prices. */
        wxPanel* pricesPanel;
        /** Market catalogue information from betfair. */
        entity::Market market;
        /** The market's runners and their prices. */
        std::map<int, RunnerRow*> runnerRows;
        /** The parent panel. */
        MarketPanels* marketPanels;
        /** The market name. */
        wxStaticText* marketName;
        // greentop::ListMarketBookRequest listMarketBookRequest;
        // greentop::ListMarketBookResponse listMarketBookResponse;
        /** A timer used to refresh the market's prices at intervals */
        wxTimer refreshTimer;
        /** Worker manager. */
        worker::WorkerManager workerManager;
        /** The toolbar - displays the market name, event start time, current orders + refresh + close buttons */
        wxToolBar* toolbar;
        /** The id of the "in play" tool. */
        int inPlayToolId;
        /** Display status if other than "open". */
        wxStaticText* marketStatus;
        /** Display matched and unmatched orders. */
        dialog::CurrentOrders* currentOrdersDialog;
        /** Button which opens the current orders dialog. */
        wxToolBarToolBase* currentOrdersTool;
        /** The full market name. */
        std::string fullMarketName;
        /** The market id. */
        std::string marketId;
        /** The betfair market book information. */
        greentop::MarketBook marketBook;

        /**
         * Click price button handler. Opens the place bet dialog.
         *
         * @param event The click event.
         */
        void OnClick(const wxCommandEvent& event);

        /**
         * Place bet handler.  Refreshes prices.
         *
         * @param event The worker event.
         */
        void OnPlaceBet(const wxThreadEvent& event);

        /**
         * Cancel orders handler. Refreshes prices.
         *
         * @param event The worker event.
         */
        void OnCancelOrders(const wxThreadEvent& event);

        /**
         * Refreshes prices.
         */
        void RefreshPrices();

        /**
         * Refreshes prices.
         * 
         * @param event The click event.
         */
        void RefreshPrices(const wxEvent& event);

        /**
         * Update market prices.
         *
         * @param event The worker event.
         */
        void OnMarketUpdated(const wxThreadEvent& event);

        /**
         * Close the market panel.
         *
         * @param event The click event.
         */
        void OnClickClose(const wxCommandEvent& event);

        /**
         * Add / remove runners, update prices, update profit and loss.
         */
        void SyncRunnerRows();

        /**
         * Update market profit and loss.
         *
         * @param event The worker event.
         */
        void OnListMarketProfitAndLoss(const wxThreadEvent& event);

        /**
         * Display potential profit and loss before a bet is placed.
         *
         * @param event The order pending event.
         */
        void OnPlaceOrderPending(const wxCommandEvent& event);

        /**
         * Show a green tick when the market is in-play, or a grey tick if the market is going in-play.
         */
        void UpdateToolBar();

        /**
         * Display the market's status if it is suspended or closed.
         */
        void UpdateMarketStatus();

        /**
         * Open current orders dialog.
         */
        void ShowCurrentOrders(const wxEvent& event);

        /**
         * Enable / disable button that opens the current orders dialog.
         *
         * @param event The worker event.
         */
        void OnListCurrentOrders(const wxThreadEvent& event);

        DECLARE_NO_COPY_CLASS(MarketPanel)
};

}

#endif // MARKETPANEL_H
