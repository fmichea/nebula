#pragma once
#ifndef NEBULA_MEMORY_SEGMENTS_HH_
# define NEBULA_MEMORY_SEGMENTS_HH_

# include <cassert>
# include <list>

namespace nebula::memory::segments {
    class Segment {
    public:
        Segment(bool mbc, uint16_t start, uint16_t size);
        Segment(bool mbc, uint16_t start, uint16_t size, uint16_t bank_size);
        Segment(bool mbc, uint16_t start, uint16_t end, uint16_t bank_count, bool bank0_sep);
        virtual ~Segment();

        void reset();

        bool mbc() const;

        bool contains_address(uint16_t addr) const;
        bool in_banked_zone(uint16_t addr) const;

        uint8_t* ptr(uint16_t addr) const;
        uint8_t* ptr(uint16_t addr, uint16_t bank) const;

        uint8_t* buffer() const;
        uint16_t size() const;

        uint16_t bank() const;

        void select_bank(uint16_t bank);

    private:
        bool _mbc;

        size_t _bufsize;
        uint8_t* _buffer;

        uint16_t _start;
        uint16_t _end;

        uint16_t _size;
        uint16_t _bank_size;

        uint16_t _bank;
        uint16_t _bank_count;
        uint16_t _banks_offset;
    };

# define X(Name, ...)                           \
    extern Segment Name;
# include "segments.def"
# undef X

    extern const std::list<Segment*> SEGMENTS;
}

#endif /* !NEBULA_MEMORY_SEGMENTS_HH_ */
