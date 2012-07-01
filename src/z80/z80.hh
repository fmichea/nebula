#ifndef Z___HH_
# define Z___HH_

# include <stdio.h>
# include "z80opcodes.hh"
# include "z80disassembler.hh"
# include "../graphics/gpu.hh"
# include "interrupts.hh"

class Z80
{
public:
    Z80(std::string filename);

    bool execute();

private:
    std::string     filename_;
    Z80Registers    regs_;
    MMU             mmu_;
    GPU             gpu_;
    Interrupts      int_;
};

#endif // !Z___HH_
