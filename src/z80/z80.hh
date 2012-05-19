#ifndef Z___HH_
# define Z___HH_

# include <stdio.h>
# include "z80opcodes.hh"

class Z80
{
public:
    Z80(std::string filename);

    bool execute();

private:
    std::string     filename_;
    Z80Registers    regs_;
    MMU             mmu_;
};

#endif // !Z___HH_
