/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <string>
#include <wx/string.h>

#include <greentop/sport/MarketCatalogue.h>
#include <greentop/sport/Runner.h>
#include <greentop/sport/RunnerCatalog.h>

namespace greenthumb {

#if defined _WIN32
time_t timegm(tm* t);
#endif

wxString GetCurrencySymbol(const wxString& currencyCode);

wxString GetSelectionName(
    const greentop::MarketCatalogue& marketCatalogue,
    const greentop::RunnerCatalog& runnerCatalog,
    const double handicap
);

}

#endif // UTIL_H
