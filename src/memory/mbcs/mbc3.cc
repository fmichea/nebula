#include "mbc3.hh"

namespace nms = nebula::memory::segments;

MBC3::MBC3()
    : MBC()
    , latch_ (true)
    , latched_ (false)
    , halted_ (false)
{
    this->substracted_time_ = time(NULL);
}

uint8_t* MBC3::read_ram_address(uint16_t addr)
{
    uint64_t tmp;

    // RAM bank
    if (nms::ERAM.bank() <= 0x3) {
        return nms::ERAM.ptr(addr);
    } if (nms::ERAM.bank() > 0x03 && nms::ERAM.bank() < 0x08) {
        return nms::ERAM.ptr(addr, 0);
    }

    // RTC register
    if (this->latched_) {
        tmp = this->latch_time_;
    } else if (this->halted_) {
        tmp = this->halted_time_;
    } else {
        tmp = time(NULL) - this->substracted_time_;
    }

    switch (nms::ERAM.bank()) {
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

void MBC3::bank_selector_zone1(uint16_t UNUSED(addr), uint8_t value) {
    value &= 0x7F;
    if (value == 0)
        value += 0x1;
    nms::ROM.select_bank(value);
}

void MBC3::bank_mode_select(uint8_t value) {
    if (value == 1 && !this->latch_) {
        this->latch_ = true;
        this->latched_ = !this->latched_;

        if (this->latched_) {
            this->latch_time_ = time(NULL) - this->substracted_time_;
        }
    } else {
        this->latch_ = false;
    }
}

uint8_t* MBC3::write_ram_address(uint16_t addr, uint8_t value) {
    if (nms::ERAM.bank() <= 0x3) {
        return nms::ERAM.ptr(addr);
    }

    if (nms::ERAM.bank() == 0x0C && (value & 0x20) != this->halted_) {
        this->halted_ = !this->halted_;

        if (this->halted_) { // STOP... HALT TIME
            this->halted_time_ = time(NULL);
        } else { // wake-up
            this->substracted_time_ += time(NULL) - this->halted_time_;
        }
    }
    return nullptr;
}
