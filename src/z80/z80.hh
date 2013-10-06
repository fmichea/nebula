#ifndef Z___HH_
# define Z___HH_

# include <stdio.h>

# include "graphics/gpu.hh"
# include "keyboard/keyboard.hh"
# include "sound/sound.hh"
# include "z80/interrupts.hh"
# include "z80/z80disassembler.hh"
# include "z80/z80opcodes.hh"

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
    Sound           sound_;
};

#endif // !Z___HH_
