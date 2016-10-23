#ifndef NEBULA_MEMORY_MBCS_MBC5_HH_
# define NEBULA_MEMORY_MBCS_MBC5_HH_

# include "mbc.hh"

class MBC5 : public MBC {
public:
    MBC5();

private:
    void bank_selector_zone1(uint16_t addr, uint8_t value);
    void bank_selector_zone2(uint8_t value);
};

#endif // !NEBULA_MEMORY_MBCS_MBC5_HH_
