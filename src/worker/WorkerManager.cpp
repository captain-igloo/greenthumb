#include "worker/WorkerManager.h"

namespace greenthumb {
namespace worker {

WorkerManager::WorkerManager(wxEvtHandler* parent) {

    SetNextHandler(parent);
    parent->SetPreviousHandler(this);

}

void WorkerManager::RunWorker(worker::Worker* worker) {

    int id = worker->GetManagerId();

    if (worker->Run() == wxTHREAD_NO_ERROR) {
        workers[id] = worker;
    }

}

void WorkerManager::OnTerminate(wxThreadEvent& event) {

    int workerId = event.GetId();

    auto it = workers.find(workerId);

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
