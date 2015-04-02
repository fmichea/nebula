#ifndef NEBULA_MEMORY_REGISTERS_NR10_HH_
# define NEBULA_MEMORY_REGISTERS_NR10_HH_

# include "bitproxy.hh"
# include "register.hh"

class NR10Proxy : public RegisterProxy {
public:
    NR10Proxy() : RegisterProxy() {}

    NR10Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->period = BitProxy(reg, 4, 0x7);
        this->negate = BitProxy(reg, 3, 0x1);
        this->shift = BitProxy(reg, 0, 0x7);
    }

    BitProxy period;
    BitProxy negate;
    BitProxy shift;
};

#endif // !NEBULA_MEMORY_REGISTERS_NR10_HH_
