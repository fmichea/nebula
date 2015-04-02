#ifndef NEBULA_MEMORY_MBCS_MBC2_HH_
# define NEBULA_MEMORY_MBCS_MBC2_HH_

# include "mbc.hh"

class MBC2 : public MBC
{
public:
    MBC2(void* memory);

private:
    void* write_rom_bank(uint16_t addr, uint16_t value);
    void* write_ram_bank(uint16_t addr, uint16_t value);
};

#endif // !NEBULA_MEMORY_MBCS_MBC2_HH_
