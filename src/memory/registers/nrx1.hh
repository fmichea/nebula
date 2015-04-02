#ifndef NEBULA_MEMORY_REGISTERS_NRX1_HH_
# define NEBULA_MEMORY_REGISTERS_NRX1_HH_

# include "bitproxy.hh"
# include "register.hh"

class NRX1Proxy : public RegisterProxy {
public:
    NRX1Proxy() : RegisterProxy() {}

    NRX1Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->duty = BitProxy(reg, 6, 0x03);
        this->sound_length = BitProxy(reg, 0, 0x3f);
    }

    BitProxy duty;
    BitProxy sound_length;
};

#endif // !NEBULA_MEMORY_REGISTERS_NRX1_HH_
