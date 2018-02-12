/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
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

EventTree::EventTree(wxWindow *parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,
        long style) : wxTreeCtrl(parent, id, pos, size, style | wxTR_HIDE_ROOT), workerManager(this),
        listMarketCatalogueInProgress(false) {

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
    Bind(worker::LIST_MARKET_CATALOGUE, &EventTree::OnListMarketCatalogue, this, wxID_ANY);

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
        SyncNode(rootId, GreenThumb::GetBetfairApi().getMenu().getRootNode(), true);
    } catch (const std::exception& e) {
        wxLogStatus("Failed to refresh menu");
    }
}

void EventTree::SyncNode(const wxTreeItemId& itemId, const greentop::menu::Node& node, bool recurse) {

    std::set<std::string> nodeIdsFound;
    std::set<wxTreeItemId> itemIdsToDelete;

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = GetFirstChild(itemId, cookie);

    while (childId.IsOk()) {

        MenuTreeData* childData = dynamic_cast<MenuTreeData*>(GetItemData(childId));

        if (childData && childData->valid) {
            nodeIdsFound.insert(childData->node.getId());
            if (node.hasChild(childData->node)) {
                if (recurse) {
                    SyncNode(childId, childData->node, recurse);
                }
            } else {
                itemIdsToDelete.insert(childId);
            }
        } else {
            itemIdsToDelete.insert(childId);
        }

        childId = GetNextChild(itemId, cookie);
    }

    for (std::set<wxTreeItemId>::iterator itr = itemIdsToDelete.begin(); itr != itemIdsToDelete.end(); ++itr) {
        Delete(*itr);
    }

    std::set<std::string> marketIds;

    for (auto it = node.getChildren().begin(); it != node.getChildren().end(); ++it) {

        if (nodeIdsFound.find(it->getId()) == nodeIdsFound.end()) {
            wxString name(it->getName().c_str(), wxConvUTF8);

            wxTreeItemId childItemId = AppendItem(itemId, name, -1, -1, new MenuTreeData(*it));
            if (it->getChildren().size() > 0) {
                AppendItem(childItemId, "", -1, -1, new MenuTreeData());
            }
            if (it->getType() == greentop::menu::Node::Type::MARKET) {
                marketNodes[it->getId()] = childItemId;
            }
        }

        if (it->getType() == greentop::menu::Node::Type::MARKET) {
            marketIds.insert(it->getId());
        }
    }

    if (!recurse && marketIds.size() > 0) {
        for (auto it = marketIds.begin(); it != marketIds.end(); ) {
            if (betfairMarkets->exists(*it)) {
                marketIds.erase(it++);
            } else {
                ++it;
            }
        }
        if (marketIds.size() > 0) {
            workerManager.RunWorker(new worker::ListMarketCatalogue(&workerManager, marketIds));
            listMarketCatalogueInProgress = true;
        }
    }
}

bool EventTree::ListMarketCatalogueInProgress() {
    return listMarketCatalogueInProgress;
}

void EventTree::OnListMarketCatalogue(wxThreadEvent& event) {
    listMarketCatalogueInProgress = false;

    std::map<std::string, entity::Market> markets =
        event.GetPayload<std::map<std::string, entity::Market>>();

    for (auto it = markets.begin(); it != markets.end(); ++it) {
        // check that the node is still in the tree, it could have been removed after menu refresh.
        if (!it->second.HasMarketCatalogue() && marketNodes[it->first].IsOk()) {
            // The market doesn't exist, so remove its node from the tree
            wxTreeItemId parentId = GetItemParent(marketNodes[it->first]);
            Delete(marketNodes[it->first]);
            marketNodes.erase(it->first);
            if (GetChildrenCount(parentId) == 0) {
                // The parent no longer has any children so remove it as well
                Delete(parentId);
            }
        }
    }

    event.Skip();
}

void EventTree::OnItemExpanded(const wxTreeEvent& treeEvent) {
    wxCriticalSectionLocker locker(worker::RefreshMenu::criticalSection);
    try {
        wxTreeItemId itemId = treeEvent.GetItem();
        MenuTreeData* data = dynamic_cast<MenuTreeData*>(GetItemData(itemId));
        if (data) {
            SyncNode(itemId, data->node, false);
        }
    } catch (const std::out_of_range& e) {
        // attempt to expand a node that has been removed from the tree?
        wxLogStatus("Failed to expand node");
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

void EventTree::SetBetfairMarketsCache(greentop::LRUCache<std::string, entity::Market>* betfairMarkets) {
    this->betfairMarkets = betfairMarkets;
}

}
