#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <wx/log.h>

namespace greenthumb {

class Logger {

    public:

        Logger();

        void Initialise();

        ~Logger();

    protected:

    private:

        std::fstream fs;
        wxLogChain* logChain;

};

}

#endif // LOGGER_H
