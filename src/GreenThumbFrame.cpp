/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

#include "dialog/LoginDialog.h"
#include "dialog/Settings.h"

#include "entity/Config.h"

#include "worker/GetAccountDetails.h"
#include "worker/ListMarketCatalogue.h"
#include "worker/Login.h"

#include "ArtProvider.h"
#include "MenuTreeData.h"
#include "GreenThumb.h"
#include "GreenThumbFrame.h"
#include "MarketPanel.h"
#include "Util.h"

namespace greenthumb {

const wxString GreenThumbFrame::VIEW_ACCOUNT = "account";
const wxString GreenThumbFrame::VIEW_BETTING = "betting";

GreenThumbFrame::GreenThumbFrame()
       : wxFrame(NULL, wxID_ANY, _T("Green Thumb"),
                 wxDefaultPosition, wxDefaultSize,
                 wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE | wxMAXIMIZE),
        workerManager(this), betfairMarkets(100) {

    wxIcon greenthumbIcon;
    greenthumbIcon.CopyFromBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::GREENTHUMB));
    SetIcon(greenthumbIcon);

    mainView = entity::Config::GetConfigValue<wxString>("mainView", VIEW_BETTING);

    CreateMenuBar();

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    SetSizer(sizer);

    bettingPanel = new wxSplitterWindow(this, wxID_ANY);

    bettingPanel->SetSashGravity(0.0);
    bettingPanel->SetMinimumPaneSize(200);

    wxPanel* eventTreePanel = new wxPanel(bettingPanel, wxID_ANY);

    wxBoxSizer* eventTreeSizer = new wxBoxSizer(wxVERTICAL);

    eventTree = new greenthumb::EventTree(eventTreePanel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE);
    eventTree->SetBetfairMarketsCache(&betfairMarkets);

    eventTreeSizer->Add(eventTree, 1, wxEXPAND, 0);
    eventTreePanel->SetSizer(eventTreeSizer);
    eventTreePanel->SetInitialSize(wxSize(400, -1));

    marketsPanel = new MarketPanels(bettingPanel, wxID_ANY);

    marketsPanel->FitInside();
    marketsPanel->SetScrollRate(5, 5);

    wxBoxSizer* marketsSizer = new wxBoxSizer(wxVERTICAL);
    marketsPanel->SetSizer(marketsSizer);

    bettingPanel->SplitVertically(eventTreePanel, marketsPanel, 200);

    accountPanel = new AccountPanel(this, wxID_ANY);
    accountPanel->SetBetfairMarketsCache(&betfairMarkets);

    if (mainView == VIEW_ACCOUNT) {
        bettingPanel->Show(false);
    } else {
        accountPanel->Show(false);
    }

    sizer->Add(bettingPanel, 1, wxEXPAND, 0);
    sizer->Add(accountPanel, 1, wxEXPAND, 0);

    Bind(wxEVT_TREE_ITEM_ACTIVATED, &GreenThumbFrame::OnItemActivated, this, wxID_ANY);
    workerManager.Bind(worker::GET_ACCOUNT_DETAILS);

    workerManager.Bind(worker::LIST_MARKET_CATALOGUE);
    Bind(worker::LIST_MARKET_CATALOGUE, &GreenThumbFrame::OnListMarketCatalogue, this, wxID_ANY);

    CreateStatusBar(1);

}

void GreenThumbFrame::Login() {
    dialog::LoginDialog loginDialog(NULL, wxID_ANY, "Login");

    if (loginDialog.ShowModal() == wxID_OK) {
        eventTree->SyncMenu(false);

        // get account currency if we don't already have it.
        wxString currencySymbol = GetCurrencySymbol(
            entity::Config::GetConfigValue<wxString>("accountCurrency", "?")
        );
        if (currencySymbol == "?") {
            workerManager.RunWorker(new worker::GetAccountDetails(&workerManager));
        }
    }
}

void GreenThumbFrame::CreateMenuBar() {

    wxMenuBar* menu = new wxMenuBar();

    wxMenu* fileMenu = new wxMenu();
    wxWindowID menuFileLoginId = wxWindow::NewControlId();
    fileMenu->Append(menuFileLoginId, _("Log &in..."));
    wxWindowID menuFileLogoutId = wxWindow::NewControlId();
    fileMenu->Append(menuFileLogoutId, _("Log &out"));
    wxWindowID menuFileRefreshMenuId = wxWindow::NewControlId();
    fileMenu->Append(menuFileRefreshMenuId, _("Refresh &menu"));
    wxWindowID menuFileSettingsId = wxWindow::NewControlId();
    fileMenu->Append(menuFileSettingsId, _("&Settings..."));
    fileMenu->Append(wxID_EXIT, _("E&xit"));
    menu->Append(fileMenu, _("&File"));
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuFileExit, this, wxID_EXIT);

    wxMenu* viewMenu = new wxMenu();

    wxWindowID menuViewAccountId = wxWindow::NewControlId();
    wxMenuItem* accountMenuItem = viewMenu->AppendRadioItem(menuViewAccountId, wxT("&Account"));
    wxWindowID menuViewBettingId = wxWindow::NewControlId();
    wxMenuItem* bettingMenuItem = viewMenu->AppendRadioItem(menuViewBettingId, wxT("&Betting"));

    if (mainView == VIEW_ACCOUNT) {
        accountMenuItem->Check();
    } else {
        bettingMenuItem->Check();
    }

    menu->Append(viewMenu, _("&View"));

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxID_ABOUT, _("&About"));
    menu->Append(helpMenu, _("&Help"));

    SetMenuBar(menu);

    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuFileLogin, this, menuFileLoginId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuFileLogout, this, menuFileLogoutId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuFileRefreshMenu, this, menuFileRefreshMenuId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuFileSettings, this, menuFileSettingsId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuViewAccount, this, menuViewAccountId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuViewBetting, this, menuViewBettingId);
    Bind(wxEVT_MENU, &GreenThumbFrame::OnMenuHelpAbout, this, wxID_ABOUT);

}

void GreenThumbFrame::OnMenuFileLogin(const wxCommandEvent& menuEvent) {
    Login();
}

void GreenThumbFrame::OnMenuFileLogout(const wxCommandEvent& menuEvent) {
    GreenThumb::GetBetfairApi().logout();
}

void GreenThumbFrame::OnMenuFileRefreshMenu(const wxCommandEvent& menuEvent) {
    eventTree->SyncMenu();
}

void GreenThumbFrame::OnMenuFileSettings(const wxCommandEvent& menuEvent) {
    dialog::Settings settings(NULL, wxID_ANY, "Settings");
    settings.ShowModal();
}

void GreenThumbFrame::OnMenuFileExit(const wxCommandEvent& menuEvent) {
    Close(true);
}

void GreenThumbFrame::OnMenuViewAccount(const wxCommandEvent& menuEvent) {
    bettingPanel->Show(false);
    accountPanel->Show(true);
    Layout();
    entity::Config::SetConfigValue("mainView", VIEW_ACCOUNT);
}

void GreenThumbFrame::OnMenuViewBetting(const wxCommandEvent& menuEvent) {
    accountPanel->Show(false);
    bettingPanel->Show(true);
    Layout();
    entity::Config::SetConfigValue("mainView", VIEW_BETTING);
}

void GreenThumbFrame::OnMenuHelpAbout(const wxCommandEvent& menuEvent) {

    (void) wxMessageBox(_("Green Thumb\n(c) 2017 Colin Doig"),
        _("Green Thumb"),
        wxOK | wxICON_INFORMATION);

}

void GreenThumbFrame::OnItemActivated(const wxTreeEvent& treeEvent) {
    wxTreeItemId itemId = treeEvent.GetItem();
    MenuTreeData* data = dynamic_cast<MenuTreeData*>(eventTree->GetItemData(itemId));

    if (data && data->valid && data->node.getType() == greentop::menu::Node::Type::MARKET) {
        marketsPanel->AddMarket(data->node);
        if (betfairMarkets.exists(data->node.getId())) {
            marketsPanel->SetMarket(betfairMarkets.get(data->node.getId()));
        } else {
            if (!eventTree->ListMarketCatalogueInProgress()) {
                std::set<std::string> marketIds = { data->node.getId() };
                workerManager.RunWorker(
                    new worker::ListMarketCatalogue(&workerManager, marketIds)
                );
            }
        }
    }
}

void GreenThumbFrame::OnListMarketCatalogue(const wxThreadEvent& event) {
    try {
        std::map<std::string, entity::Market> markets =
            event.GetPayload<std::map<std::string, entity::Market>>();

        for (auto it = markets.begin(); it != markets.end(); ++it) {
            if (it->second.HasMarketCatalogue()) {
                betfairMarkets.put(it->second.GetMarketCatalogue().getMarketId(), it->second);
                marketsPanel->SetMarket(it->second);
            }
        }
    } catch (const std::exception& e) {
        wxLogStatus(e.what());
    }
}

}
