#ifndef BITPROXY_HH_
# define BITPROXY_HH_

# include <stdint.h>

class BitProxy
{
public:
    BitProxy()
        : reg_ (0)
    {}

    BitProxy(uint8_t* reg, uint8_t bit, uint8_t mask)
        : reg_ (reg), bit_ (bit), mask_ (mask)
    {}

    uint8_t get() const
    {
        if (this->reg_ == nullptr)
            return 0;
        return (*this->reg_ >> this->bit_) & this->mask_;
    }

    void set(uint8_t val)
    {
        uint8_t value = (val & this->mask_) << this->bit_;

        if (this->reg_ == nullptr)
            return;
        *this->reg_ &= (~0 ^ (this->mask_ << this->bit_));
        *this->reg_ |= value;
    }

private:
    uint8_t*    reg_;
    uint8_t     bit_;
    uint8_t     mask_;
};

#endif // !BITPROXY_HH_
