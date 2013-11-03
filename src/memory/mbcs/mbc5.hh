#ifndef MBC5_HH_
# define MBC5_HH_

# include "mbc.hh"

class MBC5 : public MBC
{
public:
    MBC5(void* memory);

private:
    void* write_rom_bank(uint16_t addr, uint16_t value);
    void* write_ram_bank(uint16_t addr, uint16_t value);
};

#endif // !MBC5_HH_
