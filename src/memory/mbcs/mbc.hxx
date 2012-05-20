#ifndef MBC_HXX_
# define MBC_HXX_

# include "mbc.hh"

template<typename T>
T MBC::read(uint16_t addr)
{
    T* mem = (T*) this->read_address(addr);
    T res = *mem;
    print_debug("Read value %02X from address %04X.\n", res, addr);
    return res;
}

template<typename T>
void MBC::write(uint16_t addr, T value)
{
    T* mem = (T*) this->write_address(addr, (uint16_t) value);
    if (mem == nullptr)
        return;
    print_debug("Writing value %02X in address %04X.\n", value, addr);
    *mem = value;
}

#endif // !MBC_HXX_
