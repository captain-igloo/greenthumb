/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef UTIL_H
#define UTIL_H

#include <ctime>
#include <string>

namespace greenthumb {

#if defined _WIN32
time_t timegm(tm* t);
#endif

std::string GetCurrencySymbol(const std::string& currencyCode);

std::string DoubleToString(const double dble, const unsigned precision);

}

#endif // UTIL_H
