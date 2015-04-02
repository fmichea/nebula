#ifndef NEBULA_MEMORY_REGISTERS_NRX4_HH_
# define NEBULA_MEMORY_REGISTERS_NRX4_HH_

# include "bitproxy.hh"
# include "register.hh"

class NRX4Proxy : public RegisterProxy {
public:
    NRX4Proxy() : RegisterProxy() {}

    NRX4Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->restart = BitProxy(reg, 7, 0x1);
        this->repeat = BitProxy(reg, 6, 0x1);
        this->freq_hi = BitProxy(reg, 0, 0x7);
    }

    BitProxy restart;
    BitProxy repeat;
    BitProxy freq_hi;
};

#endif // !NEBULA_MEMORY_REGISTERS_NRX4_HH_
