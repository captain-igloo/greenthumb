#ifndef WORKER_WORKERMANAGER_H
#define WORKER_WORKERMANAGER_H

#include <map>
#include <set>

#include <wx/event.h>
#include <wx/window.h>

#include "Worker.h"

namespace greenthumb {
namespace worker {

class WorkerManager : public wxEvtHandler {
    public:
        WorkerManager(wxEvtHandler* parent);

        template<typename EventTag>
        void Bind(const EventTag& eventType) {
            wxEvtHandler::Bind(eventType, &WorkerManager::OnTerminate, this, wxID_ANY);
        }

        void RunWorker(worker::Worker* worker);

        ~WorkerManager();

    protected:
    private:
        std::map<int, Worker*> workers;
        std::set<wxEventTypeTag<wxThreadEvent> > eventTypes;

        void OnTerminate(wxThreadEvent& event);
};

}
}

#endif // WORKER_WORKERMANAGER_H
