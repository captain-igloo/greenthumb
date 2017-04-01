/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */

#include <wx/log.h>

#include "worker/WorkerManager.h"

namespace greenthumb {
namespace worker {

WorkerManager::WorkerManager(wxEvtHandler* parent) {

    SetNextHandler(parent);
    parent->SetPreviousHandler(this);

}

void WorkerManager::RunWorker(worker::Worker* worker) {

    std::string description = worker->GetDescription();
    if (description != "") {
        wxLogStatus(_(description) + _(" ..."));
    }

    int id = worker->GetManagerId();

    if (worker->Run() == wxTHREAD_NO_ERROR) {
        workers[id] = worker;
    }

}

void WorkerManager::OnTerminate(wxThreadEvent& event) {

    int workerId = event.GetId();

    auto it = workers.find(workerId);

    if (event.GetString() != "") {
        wxLogStatus(event.GetString());
    }

    if (it != workers.end()) {
        workers.erase(workerId);
    }

    event.Skip();

}

WorkerManager::~WorkerManager() {

    wxCriticalSectionLocker locker(Worker::criticalSection);

    for (auto it = workers.begin(); it != workers.end(); ++it) {
        it->second->Delete();
    }

}

}
}
