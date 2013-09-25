#ifndef MBC3_HH_
# define MBC3_HH_

# include <time.h>

# include "mbc.hh"

class MBC3 : public MBC
{
public:
    MBC3(void* memory);

private:
    void* read_ram_address(uint16_t addr);
    void* write_rom_bank(uint16_t addr, uint16_t value);
    void* write_extra_address(uint16_t addr, uint16_t value);
    void* write_ram_address(uint16_t addr, uint16_t value);

    uint8_t     rtc_reg_;
    bool        latch_;
    bool        latched_;
    uint8_t     latch_time_;
    bool        halted_;
    uint64_t    halted_time_;
    uint64_t    substracted_time_;
};

#endif // !MBC3_HH_
