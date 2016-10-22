#include "z80.hh"

static inline bool do_doublespeed_cycle(MMU& mmu, uint32_t count)
{
    // Is it time to execute a doublespeed cycle ?
    return mmu.gb_type == GBType::GB // GB: always
        || mmu.KEY1.get() >> 7 == 0 // CGB in normal mode
        || count % 2 == 0; // CGB in double speed and even frame
}

Z80::Z80(MMU* mmu, GPU* gpu, Keyboard* kb)
    : int_ (mmu, regs_), gpu_ (gpu), kb_ (kb), mmu_ (mmu)
{}

bool Z80::run()
{
    uint32_t count = 0;

    while (!this->mmu_->stopped)
    {
        uint16_t res = 0x4; // FIXME: magic number to explain.

        if (!this->regs_.halt_mode) {
            uint8_t opcode = this->mmu_->read<uint8_t>(this->regs_.PC);

            if (logging::isEnabledFor(logging::DEBUG)) {
                uint8_t mem1 = this->mmu_->read<uint8_t>(this->regs_.PC + 1);
                uint8_t mem2 = this->mmu_->read<uint8_t>(this->regs_.PC + 2);
                logging::debug("PC: %04X | OPCODE: %02X | MEM: %02X%02X",
                               this->regs_.PC, opcode, mem1, mem2);
            }

            if (OPCODES[opcode] == 0) {
                logging::error("Unknown opcodes %02X...", opcode);
                return false;
            }

            res = OPCODES[opcode](*this->mmu_, this->regs_);
        }

        this->regs_.PC += (res >> 8) & 0xff;

        // double-speed mode ?
        if (do_doublespeed_cycle(*this->mmu_, count))
            this->gpu_->do_cycle(res & 0xff);

        this->kb_->do_cycle();
        this->int_.manage_timer(res & 0xff);
        this->int_.manage_interrupts();
        count += 1;

        if (logging::isEnabledFor(logging::VERBOSE)) {
            logging::verbose("\tState after execution:");
            logging::verbose("\tR1  R2\tV1  V2");
            logging::verbose("\t%2s  %2s\t%02X  %02X", "A", "A", regs_.A, regs_.A);
            logging::verbose("\t%2s  %2s\t%02X  %02X", "B", "C", regs_.B, regs_.C);
            logging::verbose("\t%2s  %2s\t%02X  %02X", "D", "E", regs_.D, regs_.E);
            logging::verbose("\t%2s  %2s\t%02X  %02X", "H", "L", regs_.H, regs_.L);
            logging::verbose("\t%2s = %04X", "PC", regs_.PC);
            logging::verbose("\t%2s = %04X", "SP", regs_.SP);
            logging::verbose("\tFlags: Z (%u), N (%u), H (%u), C (%u)",
                           regs_.F.zf.get(), regs_.F.n.get(),
                           regs_.F.h.get(), regs_.F.cy.get());
        }
    }
    return true;
}
