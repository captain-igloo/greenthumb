/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <iomanip>
#include <iostream>
#include <sstream>

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/file.h>
#include <wx/sizer.h>
#include <wx/sstream.h>
#include <wx/stdpaths.h>
#include <wx/stattext.h>
#include <wx/wfstream.h>
#include <wx/url.h>

#include <greentop/ExchangeApi.h>

#include "dialog/PriceHistory.h"
#include "Util.h"

namespace greenthumb {
namespace dialog {

PriceHistory::PriceHistory(wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
    wxDialog(parent, id, title, pos, size, style, name) {

    int borderWidth = 10;
    int borderFlags = wxTOP | wxRIGHT | wxLEFT;

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    lastPriceTraded = new wxStaticText(this, wxID_ANY, "");

    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, borderWidth, borderWidth);
    wxStaticText* bettingOnLabel = new wxStaticText(this, wxID_ANY, "Betting on:");
    gridSizer->Add(bettingOnLabel);
    bettingOn = new wxStaticText(this, wxID_ANY, "");
    gridSizer->Add(bettingOn);

    wxStaticText* lastPriceTradedLabel = new wxStaticText(this, wxID_ANY, "Last price matched:");
    gridSizer->Add(lastPriceTradedLabel);
    lastPriceTraded = new wxStaticText(this, wxID_ANY, "");
    gridSizer->Add(lastPriceTraded);
    vbox->Add(gridSizer, 0, borderFlags, borderWidth);

    graphPanel = new ImagePanel(this, wxID_ANY, wxDefaultPosition,
        wxSize(GRAPH_WIDTH, GRAPH_HEIGHT), wxSUNKEN_BORDER);
    vbox->Add(graphPanel, 0, borderFlags, borderWidth);

    wxSizer* buttonSizer = CreateButtonSizer(wxCLOSE);
    if (buttonSizer) {
        vbox->Add(buttonSizer, 0, wxTOP | wxBOTTOM | wxALIGN_RIGHT, borderWidth);
    }

    SetSizer(vbox);
    vbox->Fit(this);

    Bind(wxEVT_BUTTON, &PriceHistory::OnClose, this, wxID_CLOSE);

}

void PriceHistory::SetLastPriceTraded(const double lastPriceTraded) {
    if (lastPriceTraded > 0) {
        std::ostringstream lastPriceStream;
        lastPriceStream << std::fixed << std::setprecision(2) << lastPriceTraded;
        wxString label((lastPriceStream.str()).c_str(), wxConvUTF8);
        this->lastPriceTraded->SetLabel(label);
    }
}

void PriceHistory::SetRunner(const entity::Market& market, const greentop::Runner& runner) {
    if (market.HasRunner(runner.getSelectionId())) {
        greentop::RunnerCatalog rc = market.GetRunner(runner.getSelectionId());
        std::string runnerName = rc.getRunnerName();
        if (runner.getHandicap().isValid()) {
            runnerName = runnerName + " "
                + wxString::Format(wxT("%.1f"), runner.getHandicap().getValue());
        }
        bettingOn->SetLabel(
            GetSelectionName(market.GetMarketCatalogue(), rc, runner.getHandicap())
        );
    }

    wxBitmap bitmap(GRAPH_WIDTH, GRAPH_HEIGHT);
    wxImage image = bitmap.ConvertToImage();
    wxString filename = GetGraphFilename(market, runner);

    if (image.LoadFile(filename, wxBITMAP_TYPE_JPEG)) {
        graph = wxBitmap(image);
        graphPanel->SetBitmap(graph);
    }
}

const wxString PriceHistory::GetGraphFilename(
    const entity::Market& market,
    const greentop::Runner& runner
) {
    wxStandardPaths sp = wxStandardPaths::Get();
    wxString filename = sp.GetTempDir() + wxT("/graph.jpeg");
    wxString marketId(market.GetMarketCatalogue().getMarketId().substr(2));

    std::ostringstream oStream;
    oStream << runner.getSelectionId().getValue();
    wxString selectionId = oStream.str();

    wxString handicap = "0";
    greentop::Optional<double> optionalHandicap = runner.getHandicap();
    if (optionalHandicap.isValid()) {
        std::ostringstream handicapStream;
        handicapStream << optionalHandicap.getValue();
        handicap = handicapStream.str();
    }

    wxURL url(wxT("http://sportsiteexweb.betfair.com.au/betting/LoadRunnerInfoChartAction.do?marketId=") +
        marketId +
        wxT("&selectionId=") +
        selectionId + "&handicap=" + handicap);

    if (url.GetError() == wxURL_NOERR) {
        wxString imageData;
        wxInputStream* in = url.GetInputStream();

        if (in && in->IsOk()) {
            wxFileOutputStream out(filename);
            in->Read(out);
        }

        delete in;
    }

    return filename;
}

void PriceHistory::OnClose(wxCommandEvent& event) {
    EndModal(wxID_OK);
}

}
}
