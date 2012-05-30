#ifndef Z___OPCODES_HH_
# define Z___OPCODES_HH_

# include "../memory/mmu.hh"
# include "z80registers.hh"
# include "z80cbopcodes.hh"

uint16_t rst_nn(MMU& mmu, Z80Registers& regs, uint16_t a);

extern op OPCODES[0x100];

#endif // !Z___OPCODES_HH_
