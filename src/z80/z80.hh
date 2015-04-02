#ifndef NEBULA_Z80_Z80_HH_
# define NEBULA_Z80_Z80_HH_

# include <stdio.h>

# include "../graphics/gpu.hh"
# include "../keyboard/keyboard.hh"
# include "interrupts.hh"
# include "z80disassembler.hh"
# include "z80opcodes.hh"

# ifdef _SOUND
#  include "../sound/sound.hh"
# endif

class Z80
{
public:
    Z80(MMU* mmu, GPU* gpu, Keyboard* kb);

    bool run();

private:
    Z80Registers    regs_;
    Interrupts      int_;

    // Components of the GameBoy which the z80 depends on.
    GPU*            gpu_;
    Keyboard*       kb_;
    MMU*            mmu_;
};

#endif // !NEBULA_Z80_Z80_HH_
