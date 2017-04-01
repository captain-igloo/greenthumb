/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <wx/log.h>

#include "worker/RefreshMenu.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(REFRESH_MENU, wxThreadEvent);

RefreshMenu::RefreshMenu(wxEvtHandler* eventHandler, const std::string& cacheFilename) :
    Worker(eventHandler), cacheFilename(cacheFilename) {
    description = "Refresh menu";
}

wxThread::ExitCode RefreshMenu::Entry() {
    wxThreadEvent* event = new wxThreadEvent(REFRESH_MENU);

    try {
        if (DoRefreshMenu()) {
            event->SetString(_(description) + _(" ... Success"));
        } else {
            event->SetString(_(description) + _(" ... Failed"));
        }

    } catch (const std::exception& e) {
        event->SetString(_(description) + _(" ... Failed: ") + _(e.what()));
    }

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

bool RefreshMenu::DoRefreshMenu() {

    GreenThumb::GetBetfairApi().refreshMenu(cacheFilename);

    return true;

}

}
}
