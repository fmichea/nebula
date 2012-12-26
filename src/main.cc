#include <iostream>
#include "z80/z80.hh"
#include "commandline.hh"

int main(int argc, const char** argv)
{
    std::string rom;
    if (!cmdline::parse(rom, argc, argv))
        return 1;
    Z80 z80(rom.c_str());
    if (!z80.execute())
        return 1;
    return 0;
}
