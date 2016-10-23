#ifndef NEBULA_MEMORY_MBCS_MBC2_HH_
# define NEBULA_MEMORY_MBCS_MBC2_HH_

# include "mbc.hh"

class MBC2 : public MBC {
public:
    MBC2();

private:
    void bank_selector_zone1(uint16_t addr, uint8_t value);
    void bank_selector_zone2(uint8_t value);
};

#endif // !NEBULA_MEMORY_MBCS_MBC2_HH_
