/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <sstream>
#include <wx/file.h>
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
    const greentop::Runner& runner
) : Worker(eventHandler),
    market(market),
    runner(runner),
    lastPriceTraded(lastPriceTraded) {

}

wxThread::ExitCode GetPriceHistoryGraph::Entry() {

    wxStandardPaths sp = wxStandardPaths::Get();
    wxString filename = sp.GetTempDir() + wxT("/graph.jpeg");
    wxString marketId(market.GetMarketCatalogue().getMarketId().substr(2));

    std::ostringstream oStream;
    oStream << runner.getSelectionId();
    wxString selectionId = oStream.str();

    wxString graphHost;
    if (market.GetExchange() == greentop::Exchange::UK) {
        graphHost = wxT("uk.site.sports.betfair.com");
    } else {
        graphHost = wxT("au.site.sports.betfair.com");
    }

    wxURL url(wxT("http://") + graphHost + wxT("/betting/LoadRunnerInfoChartAction.do?marketId=") +
        marketId +
        wxT("&selectionId=") +
        selectionId +
        wxT("&asianLineId=0"));

    wxThreadEvent* event = new wxThreadEvent(GET_PRICE_HISTORY_GRAPH);
    event->SetPayload<wxString>("");

    if (url.GetError() == wxURL_NOERR) {

        wxString imageData;
        std::unique_ptr<wxInputStream> in(url.GetInputStream());

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
