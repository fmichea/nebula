#include "mbc3.hh"

MBC3::MBC3(void* rom)
    : MBC(rom), latch_ (true), latched_(false), halted_(false)
{
    this->substracted_time_ = time(NULL);
}

void* MBC3::read_ram_address(uint16_t addr)
{
    uint64_t tmp;

    // RAM bank
    if (this->ram_bank_ <= 0x3)
        return this->ram_ + addr - 0xa000 + 0x2000 * this->ram_bank_;
    if (this->ram_bank_ > 0x03 && this->ram_bank_ < 0x08)
        return this->ram_ + addr - 0xa000;

    // RTC register
    if (this->latched_)
        tmp = this->latch_time_;
    else if (this->halted_)
        tmp = this->halted_time_;
    else
        tmp = time(NULL) - this->substracted_time_;

    switch (this->ram_bank_)
    {
    case 0x08: // seconds
        this->rtc_reg_ = tmp % 60;
        break;
    case 0x09: // minutes
        this->rtc_reg_ = (tmp / 60) % 60;
        break;
    case 0x0A: // hours
        this->rtc_reg_ = (tmp / (60 * 60)) % 24;
        break;
    case 0x0B: // days
        this->rtc_reg_ = (tmp / (60 * 60 * 24)) & 0xFF;
        break;
    case 0x0C:
        // 9th bit of day
        this->rtc_reg_ = (((tmp / (60 * 60 * 24)) & 0xFF) >> 8) & 0x01;
        // TODO: carry
        this->rtc_reg_ |= ((this->halted_ ? 1 : 0) << 6) | (0 << 7);
        break;
    }

    return &this->rtc_reg_;
}

void* MBC3::write_rom_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    value &= 0x7F;
    if (value == 0)
        value += 0x1;
    this->rom_bank_ = value;

    return NULL;
}

void* MBC3::write_extra_address(uint16_t addr, uint16_t value)
{
    (void)addr;

    if (value == 1 && !this->latch_)
    {
        this->latch_ = true;
        this->latched_ = !this->latched_;

        if (this->latched_)
            this->latch_time_ = time(NULL) - this->substracted_time_;
    }
    else
        this->latch_ = false;

    return NULL;
}

void* MBC3::write_ram_address(uint16_t addr, uint16_t value)
{
    if (this->ram_bank_ <= 0x3)
        return this->ram_ + addr - 0xa000 + this->ram_bank_ * 0x2000;

    if (this->ram_bank_ == 0x0C && (value & 0x20) != this->halted_)
    {
        this->halted_ = !this->halted_;

        if (this->halted_) // STOP... HALT TIME
            this->halted_time_ = time(NULL);
        else // wake-up
            this->substracted_time_ += time(NULL) - this->halted_time_;
    }

    return NULL;
}
