#ifndef NEBULA_Z80_Z80OPCODES_HH_
# define NEBULA_Z80_Z80OPCODES_HH_

# include "../memory/mmu.hh"
# include "../utils/utils.hh"
# include "z80cbopcodes.hh"
# include "z80registers.hh"

uint16_t di(MMU& mmu, Z80Registers& regs);
uint16_t rst_nn(MMU& mmu, Z80Registers& regs, uint16_t a);

extern op OPCODES[0x100];

#endif // !NEBULA_Z80_Z80OPCODES_HH_
