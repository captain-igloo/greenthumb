/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <iostream>

#include <wx/window.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxCriticalSection Worker::criticalSection;

int Worker::previousManagerId = 0;

Worker::Worker(wxEvtHandler* eventHandler) : eventHandler(eventHandler) {
    managerId = ++previousManagerId;
}

void Worker::QueueEvent(wxThreadEvent* threadEvent) {
    wxCriticalSectionLocker locker(criticalSection);

    if (parentAlive) {
        threadEvent->SetId(managerId);
        wxQueueEvent(eventHandler, threadEvent);
    }
}

int Worker::GetManagerId() const {
    return managerId;
}

void Worker::SetParentAlive(const bool parentAlive) {
    this->parentAlive = parentAlive;
}

}
}
