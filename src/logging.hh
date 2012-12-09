#ifndef LOGGING_HH_
# define LOGGING_HH_

# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>

namespace logging {
    void init(int level);

    int info(const char* format, ...);
    int warning(const char* format, ...);
    int error(const char* format, ...);
    int debug(const char* format, ...);
}

#endif // !LOGGING_HH_
