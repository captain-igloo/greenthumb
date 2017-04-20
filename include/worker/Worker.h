/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef WORKER_WORKER_H
#define WORKER_WORKER_H

#include <string>

#include <wx/event.h>
#include <wx/thread.h>

namespace greenthumb {
namespace worker {

/**
 * Base class for managed workers.
 */
class Worker : public wxThread {
    public:

        static wxCriticalSection criticalSection;

        /**
         * Constructor
         *
         * @param eventHandler The handler to signal on completion.
         */
        Worker(wxEvtHandler* eventHandler);

        /**
         * Returns a unique identifier for this worker.
         *
         * @return The worker's unique identifier.
         */
        int GetManagerId() const;

        /**
         * Signal that the worker has finished.
         *
         * @param threadEvent The event to queue.
         */
        void QueueEvent(wxThreadEvent* threadEvent);

        /**
         * Returns a description of this worker.
         *
         * @return The worker's description.
         */
        const std::string& GetDescription() const;

        /**
         * If the parent wxEvtHandler has been deleted, no attempt will be made to signal it when
         * the worker finishes.
         *
         * @param parentAlive False if the parent has been deleted.
         */
        void SetParentAlive(const bool parentAlive);

    protected:
        wxEvtHandler* eventHandler;
        int managerId;
        std::string description;
        bool parentAlive = true;

    private:
        static int previousManagerId;

};

}
}

#endif // WORKER_WORKER_H
