#ifndef Z__CBOPCODES_HH_
# define Z__CBOPCODES_HH_

# include <stdint.h>

# include "../memory/mmu.hh"
# include "z80registers.hh"

# define P(Size, Clock) ((Size << 8) + Clock)

typedef uint16_t (*op) (MMU&, Z80Registers&);

extern op CBOPCODES[0x100];

#endif // !Z__CBOPCODES_HH_
