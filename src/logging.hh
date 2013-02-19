#ifndef LOGGING_HH_
# define LOGGING_HH_

# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>

namespace logging {
    enum level {
        SILENT      = 0,
        INFO        = 1,
        WARNING     = 2,
        ERROR       = 3,
        VERBOSE     = 4,
        DEBUG       = 5,
    };
    void init(level lvl);

    int info(const char* format, ...);
    int warning(const char* format, ...);
    int error(const char* format, ...);
    int verbose(const char* format, ...);
    int debug(const char* format, ...);
}

#endif // !LOGGING_HH_
