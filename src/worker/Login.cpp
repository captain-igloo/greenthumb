/**
 * Copyright 2019 Colin Doig.  Distributed under the MIT license.
 */
#include "entity/Config.h"
#include "worker/Login.h"

#include "GreenThumb.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(LOGIN, wxThreadEvent);

Login::Login(wxEvtHandler* eventHandler, const std::string& applicationKey,
    const std::string& username, const std::string& password)
    : Worker(eventHandler), applicationKey(applicationKey), username(username), password(password) {
}

wxThread::ExitCode Login::Entry() {
    bool loginResult;
    wxLogStatus("Logging in ...");
    try {
        GreenThumb::GetBetfairApi().setApplicationKey(applicationKey);
        loginResult = GreenThumb::GetBetfairApi().login(username, password);

        if (loginResult) {
            bool rememberBetfairPassword = entity::Config::GetConfigValue(
                "rememberBetfairPassword",
                false
            );
            if (rememberBetfairPassword) {
                entity::Config::SetConfigValue(
                    "ssoid",
                    wxString(GreenThumb::GetBetfairApi().getSsoid().c_str(), wxConvUTF8)
                );
                entity::Config::SetConfigValue(
                    "loginTime",
                    wxString(std::to_string(time(NULL)).c_str(), wxConvUTF8)
                );
            }
        } else {
            wxLogError("Failed to log in");
        }

    } catch (std::exception const& e) {
        loginResult = false;
        wxLogError(e.what());
    }

    if (loginResult) {
        wxLogStatus("Logging in ... Success");
    } else {
        wxLogStatus("Logging in ... Failed");
    }

    wxThreadEvent* event = new wxThreadEvent(LOGIN);
    event->ResumePropagation(wxEVENT_PROPAGATE_MAX);
    event->SetPayload<bool>(loginResult);
    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

}
}
