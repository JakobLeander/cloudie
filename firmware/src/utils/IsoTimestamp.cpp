#include "IsoTimestamp.h"

namespace utils
{
    uint64_t parseIsoTimestampMs(const std::string &timestamp)
    {
        if (timestamp.length() < 24 || timestamp[4] != '-' || timestamp[7] != '-' ||
            timestamp[10] != 'T' || timestamp[13] != ':' || timestamp[16] != ':' ||
            timestamp[19] != '.' || timestamp[23] != 'Z')
        {
            return 0;
        }

        const uint32_t year = std::stoul(timestamp.substr(0, 4));
        const uint32_t month = std::stoul(timestamp.substr(5, 2));
        const uint32_t day = std::stoul(timestamp.substr(8, 2));
        const uint32_t hour = std::stoul(timestamp.substr(11, 2));
        const uint32_t minute = std::stoul(timestamp.substr(14, 2));
        const uint32_t second = std::stoul(timestamp.substr(17, 2));
        const uint32_t milliseconds = std::stoul(timestamp.substr(20, 3));

        if (year < 1970 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour > 23 || minute > 59 || second > 59)
        {
            return 0;
        }

        static const uint32_t daysBeforeMonth[] = {
            0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
        };

        const bool leapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        const uint32_t daysInMonth =
            month == 2 ? (leapYear ? 29 : 28) :
            (month == 4 || month == 6 || month == 9 || month == 11) ? 30 : 31;
        if (day > daysInMonth)
        {
            return 0;
        }

        const uint64_t daysBeforeYear = 365ULL * (year - 1970) +
            ((year - 1969) / 4) - ((year - 1901) / 100) + ((year - 1601) / 400);
        const uint64_t days = daysBeforeYear + daysBeforeMonth[month - 1] +
            (month > 2 && leapYear ? 1 : 0) + day - 1;

        return days * 86400000ULL + hour * 3600000ULL + minute * 60000ULL +
            second * 1000ULL + milliseconds;
    }
}
