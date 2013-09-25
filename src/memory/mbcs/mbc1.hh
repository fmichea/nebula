#ifndef MBC1_HH_
# define MBC1_HH_

# include "mbc.hh"

typedef enum mbc_mode
{
    MBC_MODE_ROM,
    MBC_MODE_RAM
} e_mbc_mode;


class MBC1 : public MBC
{
public:
    MBC1(void* memory);

private:
    void* read_rom_address(uint16_t addr);
    void* read_ram_address(uint16_t addr);
    void* write_rom_bank(uint16_t addr, uint16_t value);
    void* write_ram_bank(uint16_t addr, uint16_t value);
    void* write_extra_address(uint16_t addr, uint16_t value);
    void* write_ram_address(uint16_t addr, uint16_t value);

    e_mbc_mode  mbc_mode_;
};

#endif // !MBC1_HH_
