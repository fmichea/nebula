#ifndef COMMANDLINE_HH_
# define COMMANDLINE_HH_

# include <string>
# include <stdio.h>

# include "logging.hh"

namespace cmdline {
    bool parse(std::string& rom, int argc, const char** argv);
}

#endif // !COMMANDLINE_HH_
