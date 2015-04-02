#ifndef NEBULA_MEMORY_REGISTERS_NR50_HH_
# define NEBULA_MEMORY_REGISTERS_NR50_HH_

# include "bitproxy.hh"
# include "register.hh"

# include "../../sound/constants.hh"

class NR50Proxy : public RegisterProxy {
public:
    NR50Proxy() : RegisterProxy() {}

    NR50Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        for (int it = 0; it < NB_CHANNELS; ++it)
            this->volume[it] = BitProxy(reg, 4 * it, 0x7);
    }

    BitProxy volume[NB_CHANNELS];
};

#endif /* !NEBULA_MEMORY_REGISTERS_NR50_HH_ */
