#ifndef MBC_HXX_
# define MBC_HXX_

# include "memory/mbcs/mbc.hh"

template<typename T>
T MBC::read(uint16_t addr)
{
    T* mem = (T*) this->read_address(addr);
    //T res = *mem;
    //logging::verbose("Read value %04X from address %04X.", res, addr);
    return *mem;
}

template<typename T>
void MBC::write(uint16_t addr, T value)
{
    T* mem = (T*) this->write_address(addr, (uint16_t) value);
    if (mem == nullptr)
        return;
    //logging::verbose("Writing value %04X in address %04X.", value, addr);
    *mem = value;
}

#endif // !MBC_HXX_
