/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#include <wx/wx.h>
#include <wx/evtloop.h>
#include <greentop/ExchangeApi.h>

#include "GreenThumb.h"
#include "GreenThumbFrame.h"
#include "Logger.h"

#include "Db.h"
#include "entity/Config.h"

IMPLEMENT_APP(GreenThumb)

bool GreenThumb::OnInit() {

    try {

        wxInitAllImageHandlers();

        logger.Initialise();

        greenthumb::Db::InitialisePool();

        // create and show the main frame
        frame = new greenthumb::GreenThumbFrame();
        frame->Show(true);
        frame->Login();

    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
        return false;
    }

    return true;
}

bool GreenThumb::OnExceptionInMainLoop() {

    try {
        throw;
    } catch (std::exception const& e) {
        std::cout << "ERROR :: " << e.what() << std::endl;
    }

    return true;
}

void GreenThumb::OnUnhandledException() {
    try {
        throw;
    } catch (std::exception const& e) {
        std::cout << "Unhandled exception :: " << e.what() << std::endl;
    }
}


greentop::ExchangeApi& GreenThumb::GetBetfairApi() {
    static greentop::ExchangeApi betfairApi;
    return betfairApi;
}

