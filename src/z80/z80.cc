#include "z80.hh"

Z80::Z80(std::string filename)
    : filename_(filename)
{}

bool Z80::execute()
{
    if (!this->mmu_.load_rom(filename_))
        return false;
    while (true)
    {
        uint8_t opcode = this->mmu_.read<uint8_t>(this->regs_.PC);
        printf("Opcode : %02X, PC : %04X\n", opcode, this->regs_.PC);
        if (OPCODES[opcode] == 0) {
            fprintf(stderr, "Unknown opcodes %02X...", opcode);
            return false;
        }
        uint16_t res = OPCODES[opcode](this->mmu_, this->regs_);
        this->regs_.PC += (res >> 8) & 0xff;
    }
    return true;
}
