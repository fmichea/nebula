#include "mbc2.hh"

MBC2::MBC2(void* rom)
    : MBC(rom)
{}

void* MBC2::write_rom_bank(uint16_t UNUSED(addr), uint16_t value)
{
    this->rom_bank_ = value & 0x0F;
    return NULL;
}

void* MBC2::write_ram_bank(uint16_t UNUSED(addr), uint16_t UNUSED(value))
{
    // oh no, you are not changing that RAM bank number !
    return NULL;
}
