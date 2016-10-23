#ifndef NEBULA_MEMORY_MBCS_MBC1_HH_
# define NEBULA_MEMORY_MBCS_MBC1_HH_

# include "mbc.hh"

enum class MBC1Mode {
    ROM_BANKING_MODE, // up to 8KByte RAM, 2MByte ROM
    RAM_BANKING_MODE, // up to 32KByte RAM, 512KByte ROM
};

class MBC1 : public MBC {
public:
    MBC1();

    void reset();
    MBC1Mode mode() const;

private:
    void bank_selector_zone1(uint16_t addr, uint8_t value);
    void bank_selector_zone2(uint8_t value);
    void bank_mode_select(uint8_t value);

private:
    MBC1Mode _mode;
};

#endif // !NEBULA_MEMORY_MBCS_MBC1_HH_
