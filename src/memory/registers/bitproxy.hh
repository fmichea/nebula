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

    operator uint8_t ()
    {
        return (*this->reg_ >> this->bit_) & this->mask_;
    }

    BitProxy& operator= (uint8_t val)
    {
        uint8_t value = (val & this->mask_) << this->bit_;

        if (value) *this->reg_ |= value;
        else *this->reg_ &= (~0 ^ value);
        return *this;
    }

private:
    uint8_t*    reg_;
    uint8_t     bit_;
    uint8_t     mask_;
};

#endif // !BITPROXY_HH_
