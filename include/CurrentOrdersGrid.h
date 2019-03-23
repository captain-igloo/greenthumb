/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef CURRENTORDERS_H
#define CURRENTORDERS_H

#include <greentop/LRUCache.h>

#include "entity/Market.h"
#include "PaginatedGrid.h"

namespace greenthumb {

class CurrentOrdersGrid : public PaginatedGrid {

    public:
        CurrentOrdersGrid(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);

        void SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets);

    protected:
        virtual void Render();

        virtual void OnClickFirst(wxCommandEvent& event);
        virtual void OnClickPrevious(wxCommandEvent& event);
        virtual void OnClickNext(wxCommandEvent& event);
        virtual void OnClickLast(wxCommandEvent& event);

    private:
        greentop::LRUCache<std::string, entity::Market>* betfairMarkets = NULL;
        greentop::CurrentOrderSummaryReport currentOrderSummaryReport;

        void Refresh();
        void OnClickRefresh(wxCommandEvent& event);
        void OnListCurrentOrders(const wxThreadEvent& event);
        void OnListMarketCatalogue(const wxThreadEvent& event);
};

}

#endif // CURRENTORDERS_H
