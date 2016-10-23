#include "mbc.hh"

namespace nms = nebula::memory::segments;

MBC::MBC() {}
MBC::~MBC() {}

uint8_t* MBC::real_byte_ptr(AccessType type, uint16_t addr, uint8_t value) {
    switch (type) {
    case AccessType::READ:
        if (nms::ROM.contains_address(addr)) {
            return nms::ROM.ptr(addr);
        } else if (nms::ERAM.contains_address(addr)) {
            return this->read_ram_address(addr);
        }
        break;

    case AccessType::WRITE:
        // External RAM is writeable, we return a pointer to it.
        if (nms::ERAM.contains_address(addr)) {
            return this->write_ram_address(addr, value);
        }

        /*
        ** Bank Selector:
        **   Writing to the ROM is used to controller the MBC behavior.
        **
        ** Zones:
        **   0000h-1FFFh: RAM Enable, used to protect the RAM during
        **                power-down, useless in an emulator.
        **   2000h-3FFFh: ROM Bank Selector, different for every controller.
        **   4000h-5FFFh: ROM/RAM Bank Selector, different for every controller.
        **   6000h-7FFFh: MBC mode controller, for MBC1 and MBC3.
        */
        if (0x2000 <= addr && addr <= 0x3fff) {
            this->bank_selector_zone1(addr, value);
        } else if (0x4000 <= addr && addr <= 0x5fff) {
            this->bank_selector_zone2(value);
        } else if (0x6000 <= addr && addr <= 0x7fff) {
            this->bank_mode_select(value);
        }
        break;
    };
    return nullptr;
}

// Used by MBC3.
uint8_t* MBC::read_ram_address(uint16_t addr) {
    return nms::ERAM.ptr(addr);
}

void MBC::bank_selector_zone1(uint16_t UNUSED(addr), uint8_t value) {
    nms::ROM.select_bank(value);
}

void MBC::bank_selector_zone2(uint8_t value) {
    nms::ERAM.select_bank(value);
}

// used by MBC1 and MBC3
void MBC::bank_mode_select(uint8_t UNUSED(value)) {}

// Used by MBC3
uint8_t* MBC::write_ram_address(uint16_t addr, uint8_t UNUSED(value)) {
    return nms::ERAM.ptr(addr);
}
