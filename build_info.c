#include "build_info.h"

#include <stdio.h>

#ifndef ALMAZ_BUILD_SHA
#define ALMAZ_BUILD_SHA "unknown"
#endif

#ifndef ALMAZ_BUILD_DATE
#define ALMAZ_BUILD_DATE "unknown"
#endif

const char *almaz_build_sha(void) { return ALMAZ_BUILD_SHA; }
const char *almaz_build_date(void) { return ALMAZ_BUILD_DATE; }

const char *almaz_version_string(void) {
    static char buf[96];
    snprintf(buf, sizeof(buf), "almaz %s (%s)", ALMAZ_BUILD_SHA, ALMAZ_BUILD_DATE);
    return buf;
}
