#ifndef MBC_HXX_
# define MBC_HXX_

# include "mbc.hh"

template<typename T>
T MBC::read(uint16_t addr)
{
    T* mem = (T*) this->read_address(addr);
    return (*mem);
}

template<typename T>
void MBC::write(uint16_t addr, T value)
{
    T* mem = (T*) this->write_address(addr, (uint16_t) value);
    if (mem == nullptr)
        return;
    *mem = value;
}

#endif // !MBC_HXX_
