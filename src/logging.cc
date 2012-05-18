#include "logging.hh"

int print_debug(const char* format, ...)
{
#ifdef DEBUG
    va_list     args;
    int         ret;

    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    va_end(args);
    return ret;
#else
    (void) format;
    return 0;
#endif
}
