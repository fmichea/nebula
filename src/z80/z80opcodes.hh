#ifndef Z___OPCODES_HH_
# define Z___OPCODES_HH_

# include "../memory/mmu.hh"
# include "z80registers.hh"
# include "z80cbopcodes.hh"

//# define P(Size, Clock) ((Size << 8) + Clock)

//typedef uint16_t (*op) (MMU&, Z80Registers&);

extern op OPCODES[0x100];

#endif // !Z___OPCODES_HH_
