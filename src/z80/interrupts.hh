#ifndef INTERRUPTS_HH_
# define INTERRUPTS_HH_

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"
# include "z80registers.hh"
# include "z80opcodes.hh"

class Interrupts
{
public:
    Interrupts(MMU& mmu, Z80Registers& regs);
    void manage_interrupts();

private:
    MMU&            mmu_;
    Z80Registers&   regs_;
};

#endif // !INTERRUPTS_HH_
