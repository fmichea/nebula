#include "mbc2.hh"

namespace nms = nebula::memory::segments;

MBC2::MBC2() : MBC() {}

void MBC2::bank_selector_zone1(uint16_t UNUSED(addr), uint8_t value) {
    value &= 0x0F;
    if (value == 0) {
        value = 1;
    }
    nms::ROM.select_bank(value);
}

void MBC2::bank_selector_zone2(uint8_t UNUSED(value)) {
    // oh no, you are not changing that RAM bank number!
}
