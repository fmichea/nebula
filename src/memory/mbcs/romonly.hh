#ifndef NEBULA_MEMORY_MBCS_ROMONLY_HH_
# define NEBULA_MEMORY_MBCS_ROMONLY_HH_

# include "../../utils/utils.hh"
# include "mbc.hh"

class ROMOnly : public MBC
{
public:
    ROMOnly(void* rom);

protected:
    void* read_address(uint16_t addr);
    void* write_address(uint16_t addr, uint16_t value);
};

#endif // !NEBULA_MEMORY_MBCS_ROMONLY_HH_
