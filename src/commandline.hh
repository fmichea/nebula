#ifndef NEBULA_COMMANDLINE_HH_
# define NEBULA_COMMANDLINE_HH_

# include <string>
# include <stdio.h>

# include "logging.hh"

namespace cmdline {
    bool parse(std::string& rom, int argc, const char** argv);
}

#endif // !NEBULA_COMMANDLINE_HH_
