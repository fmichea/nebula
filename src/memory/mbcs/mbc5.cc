#include "mbc5.hh"

namespace nms = nebula::memory::segments;

MBC5::MBC5() : MBC() {}

void MBC5::bank_selector_zone1(uint16_t addr, uint8_t value) {
    // FIXME: why does this take a 16b value when writes are with 8b values?
    if (addr < 0x3000) {
        nms::ROM.select_bank((nms::ROM.bank() & 0x100) | value);
    } else {
        uint16_t bank = value & 0x1;
        bank <<= 8;
        bank |= nms::ROM.bank() & 0xFF;
        nms::ROM.select_bank(bank);
    }
}

void MBC5::bank_selector_zone2(uint8_t value) {
    nms::ERAM.select_bank(value & 0x0F);
}
