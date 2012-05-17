#ifndef MBC__HH_
# define MBC__HH_

# include "mbc.hh"

typedef enum mbc_mode
{
    MODE_16_8,
    MODE_4_32
} e_mbc_mode;

class MBC1 : public MBC
{
public:
    MBC1(void* memory);

protected:
    void* read_address(uint16_t addr);
    void* write_address(uint16_t addr, uint16_t value);

private:
    e_mbc_mode  mbc_mode_;
    uint8_t     rom_bank_;
    uint8_t     ram_bank_;
};

#endif // !MBC__HH_
