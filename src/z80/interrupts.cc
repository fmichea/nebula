#include "interrupts.hh"

Interrupts::Interrupts(MMU& mmu, Z80Registers& regs)
    : mmu_ (mmu), regs_ (regs), div_cycles_ (255), tima_cycles_ (255)
{}

void Interrupts::manage_interrupts()
{
    uint8_t ints[] = {0x40, 0x48, 0x50, 0x58, 0x60};

    uint8_t if_flag = this->mmu_.IF.get();
    uint8_t state = mmu_.IE.get() & if_flag;
    print_debug("IE = %02X; IF = %02X; state = %02X;\n", mmu_.IE.get(),
                if_flag, state);
    if (state)
        this->regs_.halt_mode = false;
    if (!this->regs_.IME)
        return;
    for (uint8_t it = 0; it < 5 && state; ++it)
    {
        if ((state >> it) & 0x1)
        {
            print_debug("Launching interrupt %d (%x) !\n", it, ints[it]);
            this->regs_.IME = false;
            this->mmu_.IF.set(if_flag & (~(0x1 << it)));
            this->regs_.PC -= 1;
            rst_nn(this->mmu_, this->regs_, ints[it]);
            break;
        }
    }
}

void Interrupts::manage_timer(uint8_t cycles)
{
    // Divider register, inc at a speed of 16384 Hz (every 256 cpu cycles).
    this->div_cycles_ -= cycles;
    if (this->div_cycles_ <= 0)
    {
        this->mmu_.DIV.set(this->mmu_.DIV.get() + 1);
        this->div_cycles_ += 256;
    }

    // TAC controls the behaviour of the timer.
    uint8_t tac = this->mmu_.TAC.get();
    if (tac & 0x4) // Timer is enabled.
    {
        uint8_t tima = this->mmu_.TIMA.get();
        this->tima_cycles_ -= cycles;
        if (this->tima_cycles_ <= 0)
        {
            switch (tac & 0x3) // Controls speed of timer.
            {
            case 0x0: // 4096 Hz - 1024 cpu cycles
                this->tima_cycles_ += 1024;
                break;
            case 0x1: // 262144 Hz - 16 cpu cycles
                this->tima_cycles_ += 16;
                break;
            case 0x2: // 65536 Hz - 64 cpu cycles
                this->tima_cycles_ += 64;
                break;
            case 0x3: // 16384 Hz - 256 cpu cycles
                this->tima_cycles_ += 256;
                break;
            };
            this->mmu_.TIMA.set(tima == 0xff ? this->mmu_.TMA.get() : tima + 1);
            if (tima == 0xff)
                this->mmu_.IF.set(this->mmu_.IF.get() | 0x4);
        }
    }
}
