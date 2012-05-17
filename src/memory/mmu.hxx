#ifndef MMU_HXX_
# define MMU_HXX_

# include "mmu.hh"

template<typename T>
T MMU::read(uint16_t addr)
{
    return this->mbc_->write<T>(addr);
}

template<typename T>
void MMU::write(uint16_t addr, T value)
{
    this->mbc_->write<T>(addr, value);
}

#endif // !MMU_HXX_
