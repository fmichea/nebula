#ifndef NEBULA_MEMORY_MBCS_ROMONLY_HH_
# define NEBULA_MEMORY_MBCS_ROMONLY_HH_

# include "../../utils/utils.hh"
# include "mbc.hh"

class ROMOnly : public MBC {
public:
    ROMOnly();

    uint8_t* real_byte_ptr(AccessType type, uint16_t addr, uint8_t value);
};

#endif // !NEBULA_MEMORY_MBCS_ROMONLY_HH_
