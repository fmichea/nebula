#include "mbc1.hh"

MBC1::MBC1(void* rom)
    : MBC(rom), mbc_mode_ (MBC_MODE_ROM)
{}

void* MBC1::read_rom_address(uint16_t addr)
{
    uint16_t rom_bank = this->rom_bank_;

    if (this->mbc_mode_ == MBC_MODE_ROM)
        rom_bank |= this->ram_bank_ << 5;

    return this->rom_ + addr - 0x4000 + 0x4000 * rom_bank;
}

void* MBC1::read_ram_address(uint16_t addr)
{
    if (this->mbc_mode_ == MBC_MODE_RAM)
        return this->ram_ + addr - 0xa000 + 0x2000 * this->ram_bank_;
    else
        return this->ram_ + addr - 0xa000;
}

void* MBC1::write_rom_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    value &= 0x1F;
    if (value == 0)
        value += 0x1;
    this->rom_bank_ = value;

    return NULL;
}

void* MBC1::write_ram_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    this->ram_bank_ = value & 0x3;

    return NULL;
}

void* MBC1::write_extra_address(uint16_t addr, uint16_t value)
{
    (void)addr;

    // Mode select
    this->mbc_mode_ = (value & 0x1) ? MBC_MODE_RAM : MBC_MODE_ROM;

    return NULL;
}

void* MBC1::write_ram_address(uint16_t addr, uint16_t value)
{
    (void) value;

    if (this->mbc_mode_ == MBC_MODE_RAM)
        return this->ram_ + addr - 0xa000 + this->ram_bank_ * 0x2000;
    else
        return this->ram_ + addr - 0xa000;
}
