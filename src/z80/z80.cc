#include "z80.hh"

Z80::Z80(std::string filename)
    : filename_(filename), gpu_ (mmu_)
{}

bool Z80::execute()
{
    uint32_t count = 0;

    if (!this->mmu_.load_rom(filename_))
        return false;
    while (true)
    {
        uint8_t opcode = this->mmu_.read<uint8_t>(this->regs_.PC);
        if (OPCODES[opcode] == 0) {
            fprintf(stderr, "Unknown opcodes %02X...", opcode);
            return false;
        }
        print_debug("Opcode : %02X, PC : %04X\n", opcode, this->regs_.PC);

        uint16_t res = OPCODES[opcode](this->mmu_, this->regs_);

        print_debug("\r\tR1  R2\tV1  V2\n");
        print_debug("\t%2s  %2s\t%02X  %02X\n", "A", "A", regs_.A, regs_.A);
        print_debug("\t%2s  %2s\t%02X  %02X\n", "B", "C", regs_.B, regs_.C);
        print_debug("\t%2s  %2s\t%02X  %02X\n", "D", "E", regs_.D, regs_.E);
        print_debug("\t%2s  %2s\t%02X  %02X\n", "H", "L", regs_.H, regs_.L);
        print_debug("\t%2s = %04X\n", "SP", regs_.SP);
        print_debug("\t%2s = %04X\n", "PC", regs_.PC);
        print_debug("\tFlags: Z (%u), N (%u), H (%u), C (%u)\n",
                    (uint8_t) regs_.F.zf, (uint8_t) regs_.F.n,
                    (uint8_t) regs_.F.h, (uint8_t) regs_.F.cy);

        this->regs_.PC += (res >> 8) & 0xff;
        gpu_.do_cycle(res & 0xff);
        count += 1;
    }
    return true;
}
