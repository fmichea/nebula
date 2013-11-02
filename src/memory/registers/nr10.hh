#ifndef NR___HH_
# define NR___HH_

# include "memory/registers/bitproxy.hh"
# include "memory/registers/register.hh"

class NR10Proxy : public RegisterProxy {
public:
    NR10Proxy() : RegisterProxy() {}

    NR10Proxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->sweep_time = BitProxy(reg, 4, 0x7);
        this->sweep_way = BitProxy(reg, 3, 0x1);
        this->sweep_shift = BitProxy(reg, 0, 0x7);
    }

    BitProxy sweep_time;
    BitProxy sweep_way;
    BitProxy sweep_shift;
};

#endif // !NR___HH_
