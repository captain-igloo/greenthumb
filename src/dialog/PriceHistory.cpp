/**
 * Copyright 2020 Colin Doig.  Distributed under the MIT license.
 */
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <wx/wx.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/file.h>
#include <wx/sizer.h>
#include <wx/mstream.h>
#include <wx/stdpaths.h>
#include <wx/stattext.h>
#include <wx/wfstream.h>
#include <wx/url.h>

#include <greentop/ExchangeApi.h>

#include "dialog/PriceHistory.h"
#include "Util.h"

namespace greenthumb {
namespace dialog {

size_t writeToStream(char* buffer, size_t size, size_t nitems, wxMemoryOutputStream* stream) {
    size_t realwrote = size * nitems;
    stream->Write(buffer, realwrote);
    return realwrote;
}

PriceHistory::PriceHistory(wxWindow *parent, wxWindowID id, const wxString &title,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name) :
    wxDialog(parent, id, title, pos, size, style, name) {

    int borderWidth = 10;
    int borderFlags = wxTOP | wxRIGHT | wxLEFT;

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

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

    chartPanel = new ImagePanel(
        this,
        wxID_ANY,
        wxDefaultPosition,
        wxSize(CHART_WIDTH, CHART_HEIGHT),
        wxSUNKEN_BORDER
    );
    vbox->Add(chartPanel, 0, borderFlags, borderWidth);

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

void PriceHistory::SetRunner(const entity::Market& market, const greentop::sport::Runner& runner) {
    if (market.HasRunner(runner.getSelectionId())) {
        greentop::sport::RunnerCatalog rc = market.GetRunner(runner.getSelectionId());
        std::string runnerName = rc.getRunnerName();
        if (runner.getHandicap().isValid()) {
            runnerName = runnerName + " "
                + wxString::Format(wxT("%.1f"), runner.getHandicap().getValue());
        }
        bettingOn->SetLabel(
            GetSelectionName(market.GetMarketCatalogue(), rc, runner.getHandicap())
        );
    }

    LoadChart(market, runner);
}

void PriceHistory::LoadChart(
    const entity::Market& market,
    const greentop::sport::Runner& runner
) {
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

    wxString chartUrl = "https://xtsd.betfair.com/LoadRunnerInfoChartAction/?marketId=" +
        market.GetMarketCatalogue().getMarketId().substr(2) +
        "&selectionId=" +
        selectionId +
        "&handicap=" +
        handicap;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    if (curl) {
        wxMemoryOutputStream out;
        curl_easy_setopt(curl, CURLOPT_URL, static_cast<const char*>(chartUrl.c_str()));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToStream);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            wxMemoryInputStream in(out);
            wxImage image(in, wxBITMAP_TYPE_JPEG);
            wxBitmap chart = wxBitmap(image);
            chartPanel->SetBitmap(chart);
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void PriceHistory::OnClose(wxCommandEvent& event) {
    EndModal(wxID_OK);
}

}
}
