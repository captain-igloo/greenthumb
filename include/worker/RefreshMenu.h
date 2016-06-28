#ifndef WORKER_REFRESHMENU_H
#define WORKER_REFRESHMENU_H

#include <string>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(REFRESH_MENU, wxThreadEvent);

class RefreshMenu : public Worker {
    public:
        RefreshMenu(wxEvtHandler* eventHandler, const std::string& cacheFilename);
    protected:
        virtual ExitCode Entry();
    private:
        std::string cacheFilename;

        bool DoRefreshMenu();
};

}
}

#endif // WORKER_REFRESHMENU_H
