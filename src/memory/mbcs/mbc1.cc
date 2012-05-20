#include "mbc1.hh"

MBC1::MBC1(void* rom)
    : MBC(rom), rom_bank_ (1), ram_bank_ (0)
{}

void* MBC1::read_address(uint16_t addr)
{
    // ROM Bank 00
    if (addr <= 0x3fff)
        return (this->rom_ + addr);

    // ROM Bank 01 - 7F
    if (0x4000 <= addr && addr <= 0x7fff)
        return (this->rom_ + addr + 0x4000 * (this->rom_bank_ - 1));

    // RAM Bank 00 - 03
    if (0xa000  <= addr && addr <= 0xbfff)
        return (this->ram_ + addr - 0xa000 + 0x2000 * this->ram_bank_);

    // Other cases, we just return rom.
    return (this->rom_ + addr);
}

void* MBC1::write_address(uint16_t addr, uint16_t value)
{
    // RAM Enable (FIXME)
    if (addr <= 0x1fff)
        return NULL;

    // ROM Bank Select
    if (0x2000 <= addr && addr <= 0x3fff)
    {
        this->rom_bank_ = value & 0x1f;
        if (!this->rom_bank_)
            this->rom_bank_ = 0x1;
        return NULL;
    }

    // RAM/ROM Bank Select
    if (0x4000 <= addr && addr <= 0x7fff)
    {
        if (this->mbc_mode_ == MODE_16_8)
            this->rom_bank_ |= (value & 0x3) << 5;
        else
            this->ram_bank_ |= (value & 0x3);
        return NULL;
    }

    // Mode Select
    if (0x6000 <= addr && addr <= 0x7fff)
    {
        this->mbc_mode_ = (value & 0x1) ? MODE_16_8 : MODE_4_32;
        return NULL;
    }

    // RAM Write
    if (0xa000 <= addr && addr <= 0xbfff)
        return (this->ram_ + addr - 0xa000 + this->ram_bank_ * 0x2000);

    // ROM Write
    return (this->rom_ + addr);
}
