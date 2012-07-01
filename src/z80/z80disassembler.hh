#ifndef Z__DISASSEMBLER_HH_
# define Z__DISASSEMBLER_HH_

# include "../memory/mmu.hh"
# include "z80registers.hh"

void print_disassembly(MMU& mmu, Z80Registers& regs);

#endif // !Z__DISASSEMBLER_HH_
