#include <iostream>

#include <wx/window.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxCriticalSection Worker::criticalSection;

int Worker::previousManagerId = 0;

Worker::Worker(wxEvtHandler* eventHandler) : eventHandler(eventHandler) {
    managerId = ++previousManagerId;
    // id = wxWindow::NewControlId();
    // SetId(wxWindow::NewControlId());
}

void Worker::QueueEvent(wxThreadEvent* threadEvent) {

    wxCriticalSectionLocker locker(criticalSection);

    if (!TestDestroy()) {
        threadEvent->SetId(managerId);
        // threadEvent->SetInt(id);
        wxQueueEvent(eventHandler, threadEvent);
    }


}

void Worker::QueueEvent(const wxEventTypeTag<wxThreadEvent>& eventTag) {
    QueueEvent(new wxThreadEvent(eventTag));
}

int Worker::GetManagerId() {
    return managerId;
}

}
}
