#include "mbc5.hh"

MBC5::MBC5(void* rom)
    : MBC(rom)
{}

void* MBC5::read_rom_address(uint16_t addr)
{
    return this->rom_ + addr - 0x4000 + 0x4000 * (this->rom_bank_ & 0x1FF);
}

void* MBC5::read_ram_address(uint16_t addr)
{
    return this->ram_ + addr - 0xa000 + 0x2000 * (this->ram_bank_ & 0x0F);
}

void* MBC5::write_rom_bank(uint16_t addr, uint16_t value)
{
    if (addr < 0x3000)
        this->rom_bank_ = (this->rom_bank_ & 0x100) | (value & 0xFF);
    else
        this->rom_bank_ = (this->rom_bank_ & 0xFF) | ((value & 0x1) << 8);

    return NULL;
}

void* MBC5::write_ram_address(uint16_t addr, uint16_t value)
{
    (void) value;
    return this->ram_ + addr - 0xa000 + 0x2000 * (this->ram_bank_ & 0x0F);
}
