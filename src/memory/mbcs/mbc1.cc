#include "mbc1.hh"

MBC1::MBC1(void* rom)
    : MBC(rom), mbc_mode_ (MBC1_MODE_16_8)
{}

void* MBC1::read_rom_address(uint16_t addr)
{
    return this->rom_ + addr + 0x4000 * (this->rom_bank_ - 1);
}

void* MBC1::read_ram_address(uint16_t addr)
{
    if (this->mbc_mode_ == MBC1_MODE_4_32)
        return this->ram_ + addr - 0xa000 + 0x2000 * this->ram_bank_;
    return this->ram_ + addr - 0xa000;
}

void* MBC1::write_rom_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    if (this->mbc_mode_ == MBC1_MODE_16_8)
        this->rom_bank_ &= ~0x1F;
    else
        this->rom_bank_ = 0;
    value &= 0x1F;
    if (value == 0) // ROM select 0x00, 0x20, 0x40, 0x60.
        value += 1;
    this->rom_bank_ |= value;
    return NULL;
}

void* MBC1::write_ram_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    if (this->mbc_mode_ == MBC1_MODE_16_8) {
        this->rom_bank_ &= 0x1F;
        this->rom_bank_ |= (value & 0x3) << 5;
    } else {
        this->ram_bank_ = value & 0x3;
    }
    return NULL;
}

void* MBC1::write_extra_address(uint16_t addr, uint16_t value)
{
    (void)addr;

    // Mode select
    this->mbc_mode_ = (value & 0x1) ? MBC1_MODE_4_32 : MBC1_MODE_16_8;

    return NULL;
}

void* MBC1::write_ram_address(uint16_t addr, uint16_t value)
{
    (void) value;

    if (this->mbc_mode_ == MBC1_MODE_4_32)
        return this->ram_ + addr - 0xa000 + this->ram_bank_ * 0x2000;
    else
        return this->ram_ + addr - 0xa000;
}
