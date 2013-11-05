#ifndef Z___HH_
# define Z___HH_

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
    Z80(std::string filename);

    bool execute();

private:
    std::string     filename_;
    Z80Registers    regs_;
    MMU             mmu_;
    GPU             gpu_;
    Interrupts      int_;
    Keyboard        kb_;

# ifdef _SOUND
    Sound           sound_;
# endif
};

#endif // !Z___HH_
