#include "segments.hh"
#include "../logging.hh"
#include <iostream>

namespace nebula::memory::segments {
# define X(Name, ...)                           \
    Segment Name(__VA_ARGS__);
# include "segments.def"
# undef X

    const std::list<Segment*> SEGMENTS = {
# define X(Name, ...)                           \
        &Name,
# include "segments.def"
# undef X
    };

    Segment::Segment(bool mbc, uint16_t start, uint16_t end, uint16_t bank_count, bool bank0_sep)
        : _mbc (mbc)
        , _buffer (nullptr)
        , _start (start)
        , _end (end)
        , _bank_count (bank_count)
        , _banks_offset (0)
    {
        this->_size = (end - start) + 1;
        this->_bank_size = this->_size;
        assert((this->_size & 0x1) == 0);
        // If bank 0 is separate, then first half is bank 0, and the other half
        // is bank N.
        if (bank0_sep) {
            this->_bank_size /= 2;
            this->_banks_offset = this->_bank_size;
        }
        // Buffer initialization.
        this->_bufsize = this->_bank_count * this->_bank_size;
        this->_buffer = new uint8_t[this->_bufsize];
        // Now clean this up.
        this->reset();
    }

    Segment::Segment(bool mbc, uint16_t start, uint16_t end, uint16_t bank_count)
        : Segment (mbc, start, end, bank_count, false)
    {}

    Segment::Segment(bool mbc, uint16_t start, uint16_t end)
        : Segment (mbc, start, end, 1, false)
    {}

    Segment::~Segment() {
        delete[] this->_buffer;
    }

    void Segment::reset() {
        // If banked does not start at beginning for segment, then bank0 is
        // separate which means we start on bank1 selected, otherwise we select
        // bank0.
        this->_bank = this->_banks_offset != 0 ? 1 : 0;
        // Set the full buffer to nulls.
        memset(this->_buffer, 0, this->_bufsize);
    }

    bool Segment::mbc() const {
        return this->_mbc;
    }

    uint8_t* Segment::buffer() const {
        return this->_buffer;
    }

    uint16_t Segment::size() const {
        return this->_size;
    }

    bool Segment::contains_address(uint16_t addr) const {
        return (this->_start <= addr && addr <= this->_end);
    }

    bool Segment::in_banked_zone(uint16_t addr) const {
        return (
            this->_bank_count != 1 &&
            this->_start + this->_banks_offset <= addr &&
            addr <= this->_end
        );
    }

    uint16_t Segment::bank() const {
        return this->_bank;
    }

    uint8_t* Segment::ptr(uint16_t addr) const {
        if (this->in_banked_zone(addr)) {
            return this->ptr(addr - this->_banks_offset, this->_bank);
        } else {
            return this->ptr(addr, 0);
        }
    }

    uint8_t* Segment::ptr(uint16_t addr, uint16_t bank) const {
        return this->_buffer + (addr - this->_start) + (this->_bank_size * bank);
    }

    void Segment::select_bank(uint16_t bank) {
        this->_bank = bank;
    }
}
