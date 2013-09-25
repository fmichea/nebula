#include "mbc5.hh"

MBC5::MBC5(void* rom)
    : MBC(rom)
{}

void* MBC5::write_rom_bank(uint16_t addr, uint16_t value)
{
    if (addr < 0x3000)
        this->rom_bank_ = (this->rom_bank_ & 0x100) | (value & 0xFF);
    else
        this->rom_bank_ = (this->rom_bank_ & 0xFF) | ((value & 0x1) << 8);

    return NULL;
}

void* MBC5::write_ram_bank(uint16_t addr, uint16_t value)
{
    (void) addr;

    this->ram_bank_ = value & 0x0F;

    return NULL;
}
