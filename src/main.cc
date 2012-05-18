#include <iostream>
#include "memory/mmu.hh"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << " ROM_FILE" << std::endl;
        return 1;
    }
    MMU mmu;
    if (!mmu.load_rom(argv[1]))
        return 1;
    return 0;
}
