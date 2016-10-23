#include "mbc1.hh"

namespace nms = nebula::memory::segments;

MBC1::MBC1()
    : MBC()
    , _mode (MBC1Mode::ROM_BANKING_MODE)
{}

MBC1Mode MBC1::mode() const {
    return this->_mode;
}

void MBC1::bank_selector_zone1(uint16_t UNUSED(addr), uint8_t value) {
    uint8_t rom_bank = 0;

    // In ROM banking, the upper 2 bits of the 7 bit selector are controlled
    // separately so we need to keep them here. Otherwise they are set to 0.
    if (this->_mode == MBC1Mode::ROM_BANKING_MODE) {
        rom_bank = nms::ROM.bank() & ~0x1F;
    }

    value &= 0x1F;
    if (value == 0) { // ROM select 0x00, 0x20, 0x40, 0x60.
        value += 1;
    }

    nms::ROM.select_bank(rom_bank | value);
}

void MBC1::bank_selector_zone2(uint8_t value) {
    uint8_t rom_bank = 0;

    switch (this->_mode) {
    case MBC1Mode::ROM_BANKING_MODE:
        rom_bank = nms::ROM.bank() & 0x1F;
        rom_bank |= (value & 0x3) << 5;
        nms::ROM.select_bank(rom_bank);
        break;

    case MBC1Mode::RAM_BANKING_MODE:
        nms::ERAM.select_bank(value & 0x3);
        break;
    }
}

void MBC1::bank_mode_select(uint8_t value) {
    switch (value & 1) {
    case 1:
        this->_mode = MBC1Mode::RAM_BANKING_MODE;
        nms::ROM.select_bank(nms::ROM.bank() & 0x1f);
        break;

    default:
        this->_mode = MBC1Mode::ROM_BANKING_MODE;
        nms::ERAM.select_bank(0);
        break;
    };
}
