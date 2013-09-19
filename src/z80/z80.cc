#include "z80.hh"

Z80::Z80(std::string filename)
    : filename_(filename), gpu_ (mmu_), int_ (mmu_, regs_), kb_ ()
{}

bool Z80::execute()
{
    uint32_t count = 0;

    if (!this->mmu_.load_rom(filename_))
        return false;
    while (!this->mmu_.stopped)
    {
        uint16_t res = 0x4;
        if (!this->regs_.halt_mode)
        {
            uint8_t opcode = this->mmu_.read<uint8_t>(this->regs_.PC);
            uint8_t mem1 = this->mmu_.read<uint8_t>(this->regs_.PC + 1);
            uint8_t mem2 = this->mmu_.read<uint8_t>(this->regs_.PC + 2);
            logging::verbose("PC: %04X | OPCODE: %02X | MEM: %02X%02X",
                             this->regs_.PC, opcode, mem1, mem2);
            if (OPCODES[opcode] == 0) {
                fprintf(stderr, "Unknown opcodes %02X...", opcode);
                return false;
            }
            //print_disassembly(this->mmu_, this->regs_);
            res = OPCODES[opcode](this->mmu_, this->regs_);
        }

        this->regs_.PC += (res >> 8) & 0xff;
        this->gpu_.do_cycle(res & 0xff);
        this->kb_.do_cycle(this->mmu_);
        this->int_.manage_timer(res & 0xff);
        this->int_.manage_interrupts();
        count += 1;

        logging::debug("\tState after execution:");
        logging::debug("\tR1  R2\tV1  V2");
        logging::debug("\t%2s  %2s\t%02X  %02X", "A", "A", regs_.A, regs_.A);
        logging::debug("\t%2s  %2s\t%02X  %02X", "B", "C", regs_.B, regs_.C);
        logging::debug("\t%2s  %2s\t%02X  %02X", "D", "E", regs_.D, regs_.E);
        logging::debug("\t%2s  %2s\t%02X  %02X", "H", "L", regs_.H, regs_.L);
        logging::debug("\t%2s = %04X", "PC", regs_.PC);
        logging::debug("\t%2s = %04X", "SP", regs_.SP);
        logging::debug("\tFlags: Z (%u), N (%u), H (%u), C (%u)",
                       regs_.F.zf.get(), regs_.F.n.get(),
                       regs_.F.h.get(), regs_.F.cy.get());
    }
    return true;
}
