#ifndef BGPPROXY_HH_
# define BGPPROXY_HH_

# include "memory/registers/bitproxy.hh"

class PaletteProxy
{
public:
    PaletteProxy()
    {}

    PaletteProxy(uint8_t* reg)
    {
        this->C[0] = BitProxy(reg, 0, 0x3);
        this->C[1] = BitProxy(reg, 2, 0x3);
        this->C[2] = BitProxy(reg, 4, 0x3);
        this->C[3] = BitProxy(reg, 6, 0x3);
    }

    BitProxy C[4];
};

#endif // !BGPPROXY_HH_
