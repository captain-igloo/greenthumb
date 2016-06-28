#ifndef WORKER_LOGIN_H
#define WORKER_LOGIN_H

#include <string>

#include <wx/event.h>
#include <wx/thread.h>

#include "worker/Worker.h"

namespace greenthumb {
namespace worker {

wxDECLARE_EVENT(LOGIN, wxThreadEvent);

class Login : public Worker {
    public:
        Login(wxEvtHandler* eventHandler, const std::string& applicationKey,
              const std::string& username, const std::string& password);

    protected:

        virtual ExitCode Entry();

    private:

        std::string applicationKey;
        std::string username;
        std::string password;

};

}
}

#endif // WORKER_LOGIN_H
