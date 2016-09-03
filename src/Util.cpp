#include "Util.h"

namespace greenthumb {

#if defined _WIN32
time_t timegm(tm* time) {
    time_t localTime = mktime(time);
    tm tmVal;
    gmtime_s(&tmVal, &localTime);
    int offset = (tmVal.tm_hour - time->tm_hour);
    if (offset > 12) {
        offset = 24 - offset;
    }
    time_t utc = mktime(time) - offset * 3600;
    return utc;
}
#endif

std::string GetCurrencySymbol(const std::string& currencyCode) {

    if (currencyCode == "EUR") {
        return "€";
    } else if (currencyCode == "GBP") {
        return "£";
    } else if (currencyCode == "USD" || currencyCode == "AUD" || currencyCode == "CAD" ||
        currencyCode == "HKD" || currencyCode == "SGD") {
        return "$";
    }

    return currencyCode;
}


}
