#include <iostream>
#include "z80/z80.hh"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " ROM_FILE" << std::endl;
        return 1;
    }
    Z80 z80(argv[1]);
    if (!z80.execute())
        return 1;
    return 0;
}
