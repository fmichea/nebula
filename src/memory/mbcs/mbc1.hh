#ifndef MBC1_HH_
# define MBC1_HH_

# include "memory/mbcs/mbc.hh"

typedef enum
{
    MBC1_MODE_16_8,
    MBC1_MODE_4_32,
} e_mbc1_mode;


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

    e_mbc1_mode  mbc_mode_;
};

#endif // !MBC1_HH_
