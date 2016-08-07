#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stdpaths.h>

#include <exception>
#include <time.h>

#include <greentop/ExchangeApi.h>
#include <set>
#include <vector>

#include "worker/ListMarketCatalogue.h"
#include "worker/RefreshMenu.h"

#include "EventTree.h"
#include "GreenThumb.h"
#include "MenuTreeData.h"

namespace greenthumb {

EventTree::EventTree(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
    : wxTreeCtrl(parent, id, pos, size, style | wxTR_HIDE_ROOT), workerManager(this) {

    rootId = AddRoot(wxT("Root"));

    wxStandardPaths standardPaths = wxStandardPaths::Get();
    menuCacheFilename = standardPaths.GetUserDataDir();
    if (!wxDirExists(menuCacheFilename)) {
        wxMkdir(menuCacheFilename);
    }
    menuCacheFilename += wxFileName::GetPathSeparator() + _("menu.json");

    ReadMenuCache();
    Refresh();

    Bind(wxEVT_TREE_ITEM_EXPANDED, &EventTree::OnItemExpanded, this, wxID_ANY);

    workerManager.Bind(worker::LIST_MARKET_CATALOGUE);

    Bind(worker::REFRESH_MENU, &EventTree::Refresh, this, wxID_ANY);
    workerManager.Bind(worker::REFRESH_MENU);

}

void EventTree::SyncMenu(bool force) {

    bool syncRequired = false;

    if (!force) {
        if (!wxFileExists(menuCacheFilename)) {
            syncRequired = true;
        } else {
            time_t lastModified = wxFileModificationTime(menuCacheFilename);

            if ((time(NULL) - lastModified) > 86400) {
                syncRequired = true;
            }
        }
    }

    if (syncRequired || force) {
        workerManager.RunWorker(new worker::RefreshMenu(&workerManager, std::string(menuCacheFilename.mb_str())));
    }
}

void EventTree::Refresh(const wxThreadEvent& event) {
    try {
        SyncNode(rootId, GreenThumb::GetBetfairApi().getMenu().getRootNode());
    } catch (const std::exception& e) {
        wxLogStatus(e.what());
    }
}

void EventTree::SyncNode(const wxTreeItemId& itemId, const greentop::menu::Node& node) {

    std::set<std::string> nodeIds;
    for (auto it = node.getChildren().begin(); it != node.getChildren().end(); ++it) {
        nodeIds.insert(it->getId());
    }

    std::set<std::string> nodeIdsFound;
    std::set<wxTreeItemId> itemIdsToDelete;

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = GetFirstChild(itemId, cookie);

    while (childId.IsOk()) {

        MenuTreeData* childData = static_cast<MenuTreeData*>(GetItemData(childId));

        if (childData->valid) {
            nodeIdsFound.insert(childData->node.getId());
            if (nodeIds.find(childData->node.getId()) == nodeIds.end()) {
                itemIdsToDelete.insert(childId);
            } else {
                // SyncNode(childId, childData->node);
            }
        } else {
            itemIdsToDelete.insert(childId);
        }

        childId = GetNextChild(itemId, cookie);
    }

    for (std::set<wxTreeItemId>::iterator itr = itemIdsToDelete.begin(); itr != itemIdsToDelete.end(); ++itr) {
        Delete(*itr);
    }

    std::map<greentop::Exchange, std::set<std::string>> marketIds;

    for (auto it = node.getChildren().begin(); it != node.getChildren().end(); ++it) {

        if (nodeIdsFound.find(it->getId()) == nodeIdsFound.end()) {
            wxString name(it->getName().c_str(), wxConvUTF8);

            wxTreeItemId childItemId = AppendItem(itemId, name, -1, -1, new MenuTreeData(*it));
            if (it->getChildren().size() > 0) {
                AppendItem(childItemId, "", -1, -1, new MenuTreeData());
            }
        }

        if (it->getType() == greentop::menu::Node::Type::MARKET) {
            marketIds[it->getExchange()].insert(it->getId());
        }
    }

    for (auto it1 = marketIds.begin(); it1 != marketIds.end(); ++it1) {
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
            if (betfairMarkets->exists(*it2)) {
                it1->second.erase(*it2);
            }
        }

        if (it1->second.size() > 0) {
            workerManager.RunWorker(new worker::ListMarketCatalogue(&workerManager, it1->first, it1->second));
        }
    }

}

void EventTree::OnItemExpanded(wxTreeEvent& treeEvent) {
    try {
        wxTreeItemId itemId = treeEvent.GetItem();
        MenuTreeData* data = static_cast<MenuTreeData*>(GetItemData(itemId));
        SyncNode(itemId, data->node);
    } catch (const std::exception& e) {
        wxLogStatus(e.what());
    }
}

void EventTree::ReadMenuCache() {

    std::string line;
    std::string fileContents;
    std::ifstream fs(menuCacheFilename.mb_str());

    if (fs.is_open()) {
        while (getline(fs, line)) {
            fileContents += line;
        }
        fs.close();

        GreenThumb::GetBetfairApi().getMenu().fromJson(fileContents);
    }

}

void EventTree::SetBetfairMarketsCache(LRUCache<std::string, entity::Market>* betfairMarkets) {
    this->betfairMarkets = betfairMarkets;
}

}
