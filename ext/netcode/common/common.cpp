#include "common.h"

#include <stdio.h>
#include <time.h>

static void format_time(char *output, size_t len) {
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    snprintf(output, len, "[%d-%02d-%02d %d:%d:%d]",
             timeinfo->tm_year + 1900,
             timeinfo->tm_mon + 1,
             timeinfo->tm_mday,
             timeinfo->tm_hour,
             timeinfo->tm_min,
             timeinfo->tm_sec);
}

const char* log_timestamp() {
    static char timestamp[512];
    format_time(timestamp, sizeof(timestamp));
    return (const char*)timestamp;
}