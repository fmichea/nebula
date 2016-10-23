#ifndef NEBULA_MEMORY_MBCS_MBC3_HH_
# define NEBULA_MEMORY_MBCS_MBC3_HH_

# include <time.h>

# include "mbc.hh"

class MBC3 : public MBC
{
public:
    MBC3();

private:
    uint8_t* read_ram_address(uint16_t addr);
    uint8_t* write_ram_address(uint16_t addr, uint8_t value);

    void bank_selector_zone1(uint16_t addr, uint8_t value);
    void bank_mode_select(uint8_t value);

    uint8_t     rtc_reg_;
    bool        latch_;
    bool        latched_;
    uint8_t     latch_time_;
    bool        halted_;
    uint64_t    halted_time_;
    uint64_t    substracted_time_;
};

#endif // !NEBULA_MEMORY_MBCS_MBC3_HH_
