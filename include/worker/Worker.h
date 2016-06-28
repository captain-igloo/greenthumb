#ifndef WORKER_WORKER_H
#define WORKER_WORKER_H

#include <wx/event.h>
#include <wx/thread.h>

namespace greenthumb {
namespace worker {

class Worker : public wxThread {
    public:

        static wxCriticalSection criticalSection;

        Worker(wxEvtHandler* eventHandler);

        int GetManagerId();

        void QueueEvent(wxThreadEvent* threadEvent);

        void QueueEvent(const wxEventTypeTag<wxThreadEvent>& eventTag);

    protected:
        wxEvtHandler* eventHandler;
        int managerId;

    private:
        static int previousManagerId;

};

}
}

#endif // WORKER_WORKER_H
