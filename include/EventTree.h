/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#ifndef EVENTTREE_H
#define EVENTTREE_H

#include <greentop/LRUCache.h>
#include <map>
#include <wx/treectrl.h>

#include "entity/Market.h"
#include "worker/WorkerManager.h"

namespace greenthumb {

/**
 * Displays a tree of event types, events and markets.
 */
class EventTree : public wxTreeCtrl {
    public:

        /**
         * Constructor.
         *
         * @param parent The parent window.
         * @param id The window id.
         * @param pos The window position.
         * @param size The window size.
         * @param style The window style.
         */
        EventTree(wxWindow *parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style);

        /**
         * Request menu from betfair.
         *
         * @param force If false, the menu is requested only if it's out of date.  It true, the menu is requested
         *    regardless.
         */
        void SyncMenu(bool force = true);

        /**
         * Sets the betfair markets cache.
         *
         * @param betfairMarkets The betfair markets cache.
         */
        void SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets);

        /**
         * Whether or not a list market catalogue request is currently in progress.
         *
         * @return True if a request is in progress else false.
         */
        bool ListMarketCatalogueInProgress();

    private:
        wxTreeItemId rootId;
        std::map<std::string, wxTreeItemId> competitionItemIds;
        std::map<std::string, wxTreeItemId> eventItemIds;
        worker::WorkerManager workerManager;
        wxString menuCacheFilename;
        greentop::LRUCache<std::string, entity::Market>* betfairMarkets;
        bool listMarketCatalogueInProgress;
        std::map<std::string, wxTreeItemId> marketNodes;

        EventTree();
        void Refresh();
        void Refresh(const wxThreadEvent& event);
        void OnItemExpanded(wxTreeEvent& event);
        void SyncNode(const wxTreeItemId& itemId, const greentop::menu::Node& node, bool recurse = false);
        void ReadMenuCache();
        void OnListMarketCatalogue(wxThreadEvent& event);
};

}


#endif // EVENTTREE_H
