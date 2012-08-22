#ifndef STRING_PRINTF
#define STRING_PRINTF

#include <string>
#include <vector>
#include <stdio.h>
#include <stdarg.h>

static std::string stringprintf(const char * fmt, ...) {
    std::vector<char> buf; buf.resize(1024);
    int n;
    va_list ap;
    va_start(ap, fmt);
    n = vsnprintf(&buf[0], buf.size(), fmt, ap);
    va_end(ap);
    buf.resize(n);
    return std::string(buf.begin(), buf.end());
}

#endif /* STRING_PRINTF */
