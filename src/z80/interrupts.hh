#ifndef INTERRUPTS_HH_
# define INTERRUPTS_HH_

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"
# include "z80registers.hh"
# include "z80opcodes.hh"

# define INTERRUPT_VBLANK   (1 << 0)
# define INTERRUPT_STAT     (1 << 1)
# define INTERRUPT_TIMER    (1 << 2)
# define INTERRUPT_SERIAL   (1 << 3)
# define INTERRUPT_JOYPAD   (1 << 4)

class Interrupts
{
public:
    Interrupts(MMU& mmu, Z80Registers& regs);
    void manage_interrupts();
    void manage_timer(uint8_t cycles);

private:
    MMU&            mmu_;
    Z80Registers&   regs_;
    int16_t         div_cycles_;
    int16_t         tima_cycles_;
};

#endif // !INTERRUPTS_HH_
