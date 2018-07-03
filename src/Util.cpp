/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <iomanip>
#include <sstream>

#include "Util.h"

namespace greenthumb {

#if defined _WIN32
int32_t isLeap(int32_t year) {
    if (year % 400 == 0) {
        return 1;
    }
    if (year % 100 == 0) {
        return 0;
    }
    if (year % 4 == 0) {
        return 1;
    }
    return 0;
}

int32_t daysFrom0(int32_t year) {
    year--;
    return 365 * year + (year / 400) - (year / 100) + (year / 4);
}

int32_t daysFrom1970(int32_t year)
{
    static const int daysFrom0To1970 = daysFrom0(1970);
    return daysFrom0(year) - daysFrom0To1970;
}

int32_t daysFrom1Jan(int32_t year, int32_t month, int32_t day) {
    static const int32_t days[2][12] = {
        { 0,31,59,90,120,151,181,212,243,273,304,334 },
        { 0,31,60,91,121,152,182,213,244,274,305,335 }
    };
    return days[isLeap(year)][month - 1] + day - 1;
}

time_t timegm(tm* t) {
    int year = t->tm_year + 1900;
    int month = t->tm_mon;
    if (month > 11) {
        year += month / 12;
        month %= 12;
    } else if (month < 0) {
        int years_diff = (-month + 11) / 12;
        year -= years_diff;
        month += 12 * years_diff;
    }
    month++;
    int day = t->tm_mday;
    int dayOfYear = daysFrom1Jan(year, month, day);
    int daysSinceEpoch = daysFrom1970(year) + dayOfYear;

    time_t seconds_in_day = 3600 * 24;
    time_t result = seconds_in_day * daysSinceEpoch + 3600 * t->tm_hour + 60 * t->tm_min + t->tm_sec;

    return result;
}

#endif

wxString GetCurrencySymbol(const wxString& currencyCode) {
    if (currencyCode == "EUR") {
        return wxString("€", wxConvUTF8);
    } else if (currencyCode == "GBP") {
        return wxString("£", wxConvUTF8);
    } else if (currencyCode == "USD" || currencyCode == "AUD" || currencyCode == "CAD" ||
        currencyCode == "HKD" || currencyCode == "SGD") {
        return "$";
    }

    return currencyCode;
}

wxString GetSelectionName(
    const greentop::MarketCatalogue& marketCatalogue,
    const greentop::RunnerCatalog& runnerCatalog,
    const double handicap
) {
    wxString runnerName = runnerCatalog.getRunnerName();

    if (marketCatalogue.getDescription().getBettingType() == greentop::MarketBettingType::ASIAN_HANDICAP_DOUBLE_LINE) {
        wxString sign = "";
        if (handicap > 0) {
            sign = "+";
        }
        runnerName += " " + sign + wxString::Format(wxT("%.1f"), handicap);
    }
    return runnerName;
}

}
