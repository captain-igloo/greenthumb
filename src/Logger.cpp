#include "Logger.h"

#include <fstream>
#include <iostream>
#include <time.h>

#include <wx/filename.h>
#include <wx/stdpaths.h>

namespace greenthumb {

Logger::Logger() {

}

void Logger::Initialise() {

    wxStandardPaths standardPaths = wxStandardPaths::Get();
    wxString logFilename = standardPaths.GetUserDataDir();
    if (!wxDirExists(logFilename)) {
        wxMkdir(logFilename);
    }
    logFilename += wxFileName::GetPathSeparator() + _("log.txt");

    fs.open(logFilename.ToStdString(), std::fstream::trunc | std::fstream::out);

    logChain = new wxLogChain(new wxLogStream(&fs));

}

Logger::~Logger() {
    delete wxLog::SetActiveTarget(NULL);
}

}
