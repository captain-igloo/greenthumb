/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include "worker/Login.h"

#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LOGIN, wxThreadEvent);

Login::Login(wxEvtHandler* eventHandler, const std::string& applicationKey,
    const std::string& username, const std::string& password)
    : Worker(eventHandler), applicationKey(applicationKey), username(username), password(password) {
    description = "Login";
}

wxThread::ExitCode Login::Entry() {
    wxThreadEvent* event = new wxThreadEvent(LOGIN);

    bool loginResult;

    try {
        GreenThumb::GetBetfairApi().setApplicationKey(applicationKey);
        loginResult = GreenThumb::GetBetfairApi().login(username, password);

        if (!loginResult) {
            wxLogError("Failed to log in");
        }

    } catch (std::exception const& e) {
        loginResult = false;
        wxLogError(e.what());
    }

    if (loginResult) {
        event->SetString(_(description) + _(" ... Success"));
    } else {
        event->SetString(_(description) + _(" ... Failed"));
    }


    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<bool>(loginResult);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;

}


}
}
