/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef GREENTOPFRAME_H
#define GREENTOPFRAME_H

#include <greentop/LRUCache.h>
#include <map>
#include <string>

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/splitter.h>

#include "entity/Market.h"

#include "AccountPanel.h"
#include "EventTree.h"
#include "MarketPanels.h"

namespace greenthumb {

/**
 * The main frame.
 */
class GreenThumbFrame : public wxFrame {

    public:

        /**
         * Constructor.
         */
        GreenThumbFrame();

        /**
         * Open the login dialog
         */
        void Login();

    private:

        const static wxString VIEW_ACCOUNT;
        const static wxString VIEW_BETTING;

        /** Displays the betfair events and markets. */
        EventTree* eventTree;
        /** Displays market prices. */
        MarketPanels* marketsPanel;
        /** Displays the user's account statement. */
        AccountPanel* accountPanel;
        /** Displays the event / market navigation on the left and market prices on the right. */
        wxSplitterWindow* bettingPanel;
        /** The current view, either account or betting. */
        wxString mainView;
        /** Manages the GetAccountDetails worker */
        worker::WorkerManager workerManager;
        /** Caches betfair market catalogues. */
        greentop::LRUCache<std::string, entity::Market> betfairMarkets;

        /**
         * Creates the menu and sets up event handlers.
         */
        void CreateMenuBar();

        /**
         * File / Login handler - opens the login dialog.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuFileLogin(const wxCommandEvent& menuEvent);

        /**
         * File / Logout handler - logs out from betfair.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuFileLogout(const wxCommandEvent& menuEvent);

        /**
         * File / Refresh menu handler - download event / market tree from betfair and sync display.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuFileRefreshMenu(const wxCommandEvent& menuEvent);

        /**
         * File / Settings handler - open settings dialog.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuFileSettings(const wxCommandEvent& menuEvent);

        /**
         * File / Exit handler - exit the application.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuFileExit(const wxCommandEvent& menuEvent);

        /**
         * View / Account handler - switch to account view.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuViewAccount(const wxCommandEvent& menuEvent);

        /**
         * View / Betting handler - switch to betting view.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuViewBetting(const wxCommandEvent& menuEvent);

        /**
         * Help / About handler - open help/about dialog.
         *
         * @param menuEvent The menu event.
         */
        void OnMenuHelpAbout(const wxCommandEvent& menuEvent);

        /**
         * Open market when the user double clicks on a market in the tree.
         *
         * @param treeEvent The tree event.
         */
        void OnItemActivated(const wxTreeEvent& treeEvent);

        /**
         * Cache market catalogue(s) and update display.
         *
         * @param event The worker event.
         */
        void OnListMarketCatalogue(const wxThreadEvent& event);

        DECLARE_NO_COPY_CLASS(GreenThumbFrame)
};

}

#endif // GREENTOPFRAME_H
