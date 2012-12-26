#include "commandline.hh"

namespace {
    void print_help() {
        printf("Nebula - GameBoy Emulator\n\n");
        printf("    -h, --help          display this help\n");
        printf("    -r, --rom [rom]     GameBoy ROM to execute.\n");
        printf("    -s, --silent        no message will be displayed.\n");
        printf("    -v, --verbose       display verbose messages.\n");
        printf("    -d, --debug         display debug messages.\n");
    }

    bool is_command(std::string arg, std::string shrt, std::string lng) {
        return (arg == ("-" + shrt) || arg == ("--" + lng));
    }
}

namespace cmdline {
    bool parse(std::string& rom, int argc, const char** argv) {
        bool found_rom = false;

        for (int it = 1; it < argc; ++it) {
            if (is_command(argv[it], "h", "help")) {
                print_help();
                return false;
            } else if (is_command(argv[it], "r", "rom")) {
                if (it == argc - 1) {
                    logging::error("ROM option requires a paramter.");
                    return false;
                }
                rom = argv[++it];
                found_rom = true;
            } else if (is_command(argv[it], "s", "silent")) {
                logging::init(logging::SILENT);
            } else if (is_command(argv[it], "d", "debug")) {
                logging::init(logging::DEBUG);
            } else if (is_command(argv[it], "v", "verbose")) {
                logging::init(logging::VERBOSE);
            } else {
                logging::error("Unknown command `%s`.", argv[it]);
                return false;
            }
        }
        if (!found_rom) {
            logging::error("You must precise a ROM to load.");
            print_help();
        }
        return found_rom;
    }
}
