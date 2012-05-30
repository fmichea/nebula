#include "interrupts.hh"

Interrupts::Interrupts(MMU& mmu, Z80Registers& regs)
    : mmu_ (mmu), regs_ (regs)
{}

void Interrupts::manage_interrupts()
{
    uint8_t ints[] = {0x40, 0x48, 0x50, 0x58, 0x60};

    uint8_t state = mmu_.IE.get() & mmu_.IF.get();
    for (uint8_t it = 0; it < 5 && state; ++it)
    {
        if ((state >> it) & 0x1)
        {
            this->mmu_.IF.set(0);
            this->regs_.PC -= 1;
            rst_nn(this->mmu_, this->regs_, ints[it]);
            break;
        }
    }
}
