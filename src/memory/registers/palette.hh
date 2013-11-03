#ifndef BGPPROXY_HH_
# define BGPPROXY_HH_

# include "bitproxy.hh"
# include "register.hh"

# define _NB_COLORS 4

class PaletteProxy : public RegisterProxy {
public:
    PaletteProxy() : RegisterProxy() {}

    PaletteProxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        for (int it = 0; it < _NB_COLORS; ++it)
            this->C[it] = BitProxy(reg, it * 2, 0x3);
    }

    BitProxy C[_NB_COLORS];
};

#endif // !BGPPROXY_HH_
