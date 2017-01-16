/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_WORKERMANAGER_H
#define WORKER_WORKERMANAGER_H

#include <map>
#include <set>

#include <wx/event.h>
#include <wx/window.h>

#include "Worker.h"

namespace greenthumb {
namespace worker {

/**
 * Manage worker threads.  Typically a wxEvtHandler derived object will run a worker and bind one of it's methods so it
 * will be informed when the worker finishes.  If the event handler is destroyed while the worker is running a seg
 * fault will occur when the worker attempts to call the bound method.  We avoid this by detecting that the event
 * handler has been destroyed and signal the worker.
 */
class WorkerManager : public wxEvtHandler {
    public:

        /**
         * Constructor.
         *
         * @param parent The event handler.
         */
        WorkerManager(wxEvtHandler* parent);

        /**
         * Binds events of the given type.
         *
         * @param eventType The event type.
         */
        template<typename EventTag>
        void Bind(const EventTag& eventType) {
            wxEvtHandler::Bind(eventType, &WorkerManager::OnTerminate, this, wxID_ANY);
        }

        /**
         * Runs the given worker.
         *
         * @param worker The worker to run.
         */
        void RunWorker(worker::Worker* worker);

        /**
         * Destructor.
         */
        ~WorkerManager();

    protected:
    private:
        /** Worker threads that are being managed. */
        std::map<int, Worker*> workers;

        /**
         * Remove the worker and allow the event to propagate.
         *
         * @param event The worker event.
         */
        void OnTerminate(wxThreadEvent& event);
};

}
}

#endif // WORKER_WORKERMANAGER_H
