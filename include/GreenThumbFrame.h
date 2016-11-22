#ifndef GREENTOPFRAME_H
#define GREENTOPFRAME_H

#include <greentop/LRUCache.h>
#include <map>
#include <string>

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/splitter.h>

#include "dialog/LoginDialog.h"
#include "entity/Market.h"

#include "AccountPanel.h"
#include "EventTree.h"
#include "MarketPanels.h"

namespace greenthumb {

class GreenThumbFrame: public wxFrame {

    public:

        GreenThumbFrame();
        void Login();

        virtual ~GreenThumbFrame();

    private:

        const static std::string VIEW_ACCOUNT;
        const static std::string VIEW_BETTING;

        EventTree* eventTree;
        MarketPanels* marketsPanel;
        AccountPanel* accountPanel;
        wxSplitterWindow* bettingPanel;
        std::string mainView;
        worker::WorkerManager workerManager;
        greentop::LRUCache<std::string, entity::Market> betfairMarkets;


        void CreateMenuBar();

        void OnMenuFileLogin(wxCommandEvent& menuEvent);
        void OnMenuFileLogout(wxCommandEvent& menuEvent);
        void OnMenuFileRefreshMenu(wxCommandEvent& menuEvent);
        void OnMenuFileExit(wxCommandEvent& menuEvent);

        void OnMenuViewAccount(wxCommandEvent& menuEvent);
        void OnMenuViewBetting(wxCommandEvent& menuEvent);

        void OnMenuHelpAbout(wxCommandEvent& menuEvent);

        void OnSelChanged(wxTreeEvent& treeEvent);

        void SetMarket(wxThreadEvent& event);

        void OnListMarketCatalogue(const wxThreadEvent& event);

        DECLARE_NO_COPY_CLASS(GreenThumbFrame)
};

}

#endif // GREENTOPFRAME_H
