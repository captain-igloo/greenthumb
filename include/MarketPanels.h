/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef MARKETPANELS_H
#define MARKETPANELS_H

#include <map>
#include <string>

#include <wx/wx.h>
#include <wx/scrolwin.h>

#include "entity/Market.h"
#include "MarketPanel.h"

namespace greenthumb {

class MarketPanel;

/**
 * Container for market panels.
 */
class MarketPanels : public wxScrolled<wxPanel> {
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
        MarketPanels(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxHSCROLL | wxVSCROLL,
            const wxString& name = wxEmptyString);

        /**
         * Create and add a market if it isn't already present.  Otherwise do nothing.
         *
         * @param node The market's menu node.
         */
        void AddMarket(const greentop::menu::Node& node);

        /**
         * Pass the betfair market catalogue information to the market if it is present.
         *
         * @param market The market catalogue.
         */
        void SetMarket(const entity::Market& market);

        /**
         * Remove the market with the given id if it is present.
         *
         * @param marketId The market id.
         */
        void RemoveMarket(const std::string& marketId);

    protected:
    private:
        /** The market panels indexed by market id */
        std::map<std::string, MarketPanel*> marketPanels;

    DECLARE_NO_COPY_CLASS(MarketPanels)
};

}

#endif // MARKETPANELS_H
