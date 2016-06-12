#ifndef GREENTOP_H
#define GREENTOP_H

#include <greentop/ExchangeApi.h>
#include <string>
#include <wx/wx.h>

#include "GreenThumbFrame.h"
#include "Logger.h"

class GreenThumb : public wxApp {
    public:

        static greentop::ExchangeApi& GetBetfairApi();

        static std::string GetCurrencySymbol(const std::string& currencyCode);

        GreenThumb() {}

        virtual bool OnInit();

        virtual bool OnExceptionInMainLoop();

        virtual void OnUnhandledException();

    private:

        greenthumb::Logger logger;
        greenthumb::GreenThumbFrame* frame;

        DECLARE_NO_COPY_CLASS(GreenThumb)
};

#endif
