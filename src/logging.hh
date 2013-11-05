#ifndef LOGGING_HH_
# define LOGGING_HH_

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>

namespace logging {
    enum level {
        SILENT      = 0,
        INFO        = 1,
        WARNING     = 2,
        ERROR       = 3,
        DEBUG       = 4,
        VERBOSE     = 5,
    };

    void init(level lvl);

    bool isEnabledFor(level lvl);

    int info(const char* format, ...);
    int warning(const char* format, ...);
    int error(const char* format, ...);
    int debug(const char* format, ...);
    int verbose(const char* format, ...);
}

#endif // !LOGGING_HH_
