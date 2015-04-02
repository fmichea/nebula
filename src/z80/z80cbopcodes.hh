#ifndef NEBULA_Z80_Z80CBOPCODES_HH_
# define NEBULA_Z80_Z80CBOPCODES_HH_

# include <stdint.h>

# include "../memory/mmu.hh"
# include "z80registers.hh"

# define P(Size, Clock) ((Size << 8) + Clock)

typedef uint16_t (*op) (MMU&, Z80Registers&);

extern op CBOPCODES[0x100];

#endif // !NEBULA_Z80_Z80CBOPCODES_HH_
