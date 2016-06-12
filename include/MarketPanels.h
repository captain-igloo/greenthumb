#ifndef MARKETPANELS_H
#define MARKETPANELS_H

#include <map>
#include <string>

#include <wx/scrolwin.h>

#include "entity/Market.h"
#include "MarketPanel.h"

namespace greenthumb {

class MarketPanel;

class MarketPanels : public wxScrolled<wxPanel> {
    public:
        MarketPanels();

        MarketPanels(wxWindow* parent, wxWindowID = -1, const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize, long style=wxHSCROLL|wxVSCROLL,
                     const wxString &name="scrolledWindow");

        void AddMarket(const greentop::menu::Node& node);
        void SetMarket(const entity::Market& market);
        void RemoveMarket(const std::string& marketId);

    protected:
    private:

        std::map<std::string, MarketPanel*> marketPanels;
};

}

#endif // MARKETPANELS_H
