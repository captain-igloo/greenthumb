/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_REFRESHMENU_H
#define WORKER_REFRESHMENU_H

#include <string>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(REFRESH_MENU, wxThreadEvent);

/**
 * Refreshes the navigation menu.
 */
class RefreshMenu : public Worker {
    public:
        static wxCriticalSection criticalSection;

        /**
         * Constructor.
         *
         * @param eventHandler The handler to signal on completion.
         * @param cacheFilename The filename of the menu cache.
         */
        RefreshMenu(wxEvtHandler* eventHandler, const std::string& cacheFilename);
    protected:

        /**
         * Refreshes the navigation menu.
         */
        virtual ExitCode Entry();

    private:
        std::string cacheFilename;

        /**
         * Retrieves the navigation menu JSON from betfair and parses it.
         */
        bool DoRefreshMenu();
};

}
}

#endif // WORKER_REFRESHMENU_H
