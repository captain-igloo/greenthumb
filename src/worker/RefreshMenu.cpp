/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/log.h>

#include "worker/RefreshMenu.h"
#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(REFRESH_MENU, wxThreadEvent);

wxCriticalSection RefreshMenu::criticalSection;

RefreshMenu::RefreshMenu(wxEvtHandler* eventHandler, const std::string& cacheFilename) :
    Worker(eventHandler), cacheFilename(cacheFilename) {
}

wxThread::ExitCode RefreshMenu::Entry() {
    wxLogStatus("Refresh menu ...");

    try {
        if (DoRefreshMenu()) {
            wxLogStatus("Refresh menu ... Success");
        } else {
            wxLogStatus("Refresh menu ... Failed");
        }

    } catch (const std::exception& e) {
        wxLogStatus("Refresh menu ... Failed: " + _(e.what()));
    }

    wxThreadEvent* event = new wxThreadEvent(REFRESH_MENU);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

bool RefreshMenu::DoRefreshMenu() {
    bool result = false;
    if (GreenThumb::GetBetfairApi().retrieveMenu(cacheFilename)) {
        wxCriticalSectionLocker locker(RefreshMenu::criticalSection);
        result = GreenThumb::GetBetfairApi().parseMenu();
    }
    return result;
}

}
}
