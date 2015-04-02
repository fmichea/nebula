#ifndef NEBULA_MEMORY_REGISTERS_NRX2_HH_
# define NEBULA_MEMORY_REGISTERS_NRX2_HH_

# include "bitproxy.hh"
# include "register.hh"

class NRX2Proxy : public RegisterProxy {
public:
    NRX2Proxy() : RegisterProxy() {}

    NRX2Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->volume = BitProxy(reg, 4, 0xf);
        this->envelop_way = BitProxy(reg, 3, 0x1);
        this->envelop_sweep = BitProxy(reg, 0, 0x7);
    }

    BitProxy volume;
    BitProxy envelop_way;
    BitProxy envelop_sweep;
};

#endif // !NEBULA_MEMORY_REGISTERS_NRX2_HH_
