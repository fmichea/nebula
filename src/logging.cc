#include "logging.hh"

namespace {
    logging::level log_lvl = logging::ERROR;

    int log(logging::level lvl, FILE* stream, const char* color,
            const char* type_name, const char* format, va_list args)
    {
#ifndef RELEASE
        if (log_lvl < lvl) {
            return 0;
        }

        int ret = 0;

        if (isatty(fileno(stream)))
            ret += fprintf(stream, "%s", color);
        ret += fprintf(stream, "[%-5s] ", type_name);
        ret += vfprintf(stream, format, args);
        ret += fprintf(stream, "\n");
        if (isatty(fileno(stream)))
            ret += fprintf(stream, "\x1b[0m");
        fflush(stream);
        return ret;
#else
        (void) args;
        (void) format;
        (void) level;
        (void) stream;
        (void) type_name;
        return 0;
#endif
    }
}

namespace logging {
    void init(level lvl) {
        log_lvl = lvl;
    }

#define X(Level, FuncName, DisplayName, Color, Stream)                  \
    int FuncName(const char* format, ...) {                             \
        int     ret;                                                    \
        va_list args;                                                   \
                                                                        \
        va_start(args, format);                                         \
        ret = log(Level, Stream, Color, #DisplayName, format, args);    \
        va_end(args);                                                   \
        return ret;                                                     \
    }
X(logging::INFO, info, INFO, "", stdout)
X(logging::WARNING, warning, WARN, "\x1b[0;33m", stdout)
X(logging::ERROR, error, ERROR, "\x1b[0;31m", stderr)
X(logging::DEBUG, debug, DEBUG, "", stderr)
X(logging::VERBOSE, verbose, VERB, "", stderr)
#undef X
}
