#ifndef NEBULA_Z80_Z80DISASSEMBLER_HH_
# define NEBULA_Z80_Z80DISASSEMBLER_HH_

# include "../memory/mmu.hh"
# include "z80registers.hh"

void print_disassembly(MMU& mmu, Z80Registers& regs);

#endif // !NEBULA_Z80_Z80DISASSEMBLER_HH_
