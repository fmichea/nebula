#ifndef NEBULA_MEMORY_REGISTERS_NR52_HH_
# define NEBULA_MEMORY_REGISTERS_NR52_HH_

# include "bitproxy.hh"
# include "register.hh"

# include "../../sound/constants.hh"

class NR52Proxy : public RegisterProxy {
public:
    NR52Proxy() : RegisterProxy() {}

    NR52Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->sound_on = BitProxy(reg, 7, 0x1);
        for (unsigned int it = 0; it < NB_GB_CHANNELS; ++it)
            this->channel_on[it] = BitProxy(reg, it, 0x1);
    }

    BitProxy sound_on;
    BitProxy channel_on[NB_GB_CHANNELS];
};

#endif // !NEBULA_MEMORY_REGISTERS_NR52_HH_
