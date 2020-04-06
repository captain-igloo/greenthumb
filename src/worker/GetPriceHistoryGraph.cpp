/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include <sstream>
#include <wx/file.h>
#include <wx/filesys.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <wx/url.h>

#include "worker/GetPriceHistoryGraph.h"

namespace greenthumb {
namespace worker {

wxDEFINE_EVENT(GET_PRICE_HISTORY_GRAPH, wxThreadEvent);

GetPriceHistoryGraph::GetPriceHistoryGraph(
    wxEvtHandler* eventHandler,
    const entity::Market& market,
    const greentop::sport::Runner& runner
) : Worker(eventHandler),
    market(market),
    runner(runner) {

}

wxThread::ExitCode GetPriceHistoryGraph::Entry() {

    wxStandardPaths sp = wxStandardPaths::Get();
    wxString filename = sp.GetTempDir() + wxT("/graph.jpeg");
    wxString marketId(market.GetMarketCatalogue().getMarketId().substr(2));

    std::ostringstream oStream;
    oStream << runner.getSelectionId().getValue();
    wxString selectionId = oStream.str();

    wxThreadEvent* event = new wxThreadEvent(GET_PRICE_HISTORY_GRAPH);
    event->SetPayload<wxString>("");

    wxFileSystem fileSystem;

    wxString imageUrl = wxT("http://sportsiteexweb.betfair.com.au/betting/LoadRunnerInfoChartAction.do?marketId=") +
        marketId +
        wxT("&selectionId=") +
        selectionId;

    wxFSFile* file = fileSystem.OpenFile(imageUrl);

    if (file) {
        std::unique_ptr<wxInputStream> in(file->GetStream());
        if (in.get() && in->IsOk()) {
            wxFileOutputStream out(filename);
            in->Read(out);
            event->SetPayload<wxString>(filename);
        }
    }

    QueueEvent(event);

    return (wxThread::ExitCode) 0;
}

}
}
