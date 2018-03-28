/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>

#include "MarketPanels.h"

namespace greenthumb {

MarketPanels::MarketPanels(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxScrolled<wxPanel>(parent, id, pos, size, style, name) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);
}

void MarketPanels::AddMarket(const greentop::menu::Node& node) {
    if (marketPanels.find(node.getId()) == marketPanels.end()) {
        MarketPanel* marketPanel = new MarketPanel(this);
        marketPanel->SetMarket(node);
        marketPanels[node.getId()] = marketPanel;
        GetSizer()->Add(marketPanel, 0, wxEXPAND, 0);
        FitInside();
    }
}

void MarketPanels::SetMarket(const entity::Market& market) {
    std::map<std::string, MarketPanel*>::iterator it = marketPanels.find(market.GetMarketCatalogue().getMarketId());

    if (it != marketPanels.end())  {
        it->second->SetMarket(market);
    }
}

void MarketPanels::RemoveMarket(const std::string& marketId) {
    std::map<std::string, MarketPanel*>::iterator it = marketPanels.find(marketId);
    if (it != marketPanels.end()) {
        it->second->Destroy();
        marketPanels.erase(it);
        FitInside();
    }
}

}
