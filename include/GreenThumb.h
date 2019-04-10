/**
* Copyright 2016 Colin Doig.  Distributed under the MIT license.
*/
#ifndef GREENTOP_H
#define GREENTOP_H

#include <wx/wx.h>
#include <greentop/ExchangeApi.h>
#include <string>

#include "GreenThumbFrame.h"
#include "Logger.h"

class GreenThumb : public wxApp {
    public:

        static greentop::ExchangeApi& GetBetfairApi();

        GreenThumb() {}

        virtual bool OnInit();

        virtual bool OnExceptionInMainLoop();

        virtual void OnUnhandledException();

    private:

        greenthumb::Logger logger;
        greenthumb::GreenThumbFrame* frame = NULL;

        DECLARE_NO_COPY_CLASS(GreenThumb)
};

#endif
