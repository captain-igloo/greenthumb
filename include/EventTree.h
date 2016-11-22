#ifndef EVENTTREE_H
#define EVENTTREE_H

#include <greentop/LRUCache.h>
#include <map>
#include <wx/treectrl.h>

#include "entity/Market.h"
#include "worker/WorkerManager.h"
// #include "LRUCache.h"

namespace greenthumb {

class EventTree : public wxTreeCtrl {
    public:

        EventTree(wxWindow *parent, const wxWindowID id,
            const wxPoint& pos, const wxSize& size,
            long style);

        void SyncMenu(bool force = true);
        void SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets);

    private:

        wxTreeItemId rootId;
        std::map<std::string, wxTreeItemId> competitionItemIds;
        std::map<std::string, wxTreeItemId> eventItemIds;
        worker::WorkerManager workerManager;
        wxString menuCacheFilename;
        greentop::LRUCache<std::string, entity::Market>* betfairMarkets;

        EventTree();
        void Refresh(const wxThreadEvent& event = wxThreadEvent());
        void OnItemExpanded(wxTreeEvent& event);
        void SyncNode(const wxTreeItemId& itemId, const greentop::menu::Node& node, bool recurse = false);
        void ReadMenuCache();
};

}


#endif // EVENTTREE_H
