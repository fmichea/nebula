#ifndef MMU_HXX_
# define MMU_HXX_

# include "mmu.hh"

template<typename T>
T MMU::read(uint16_t addr)
{
    T* ptr = 0;

    if (addr < 0x4000) // Bank 00
        return this->mbc_->read<T>(addr);
    else if (0x4000 <= addr && addr < 0x8000) // Bank 01 .. NN
        return this->mbc_->read<T>(addr);
    else if (0x8000 <= addr && addr < 0xA000) // VRAM
        ptr = (T*) (this->vram_+ addr - 0x8000);
    else if (0xA000 <= addr && addr < 0xC000) // RAM
        return this->mbc_->read<T>(addr);
    else if (0xC000 <= addr && addr < 0xE000) // WRAM
        ptr = (T*) (this->wram_ + addr - 0xC000);
    else if (0xE000 <= addr && addr < 0xFE00) // ECHO
        ptr = (T*) (this->wram_ + addr - 0xE000);
    else if (0xFE00 <= addr && addr < 0xFEA0) // OAM
        ptr = (T*) (this->oam_ + addr - 0xFE00);
    //else if (0xFEA0 <= addr && addr < 0xFF00) // Not usable
        //return ((T) 0);
    else if (0xFF00 <= addr && addr < 0xFF80) // I/O Ports
        ptr = (T*) (this->io_ + addr - 0xFF00);
    else if (0xFF80 <= addr)
        ptr = (T*) (this->hram_ + addr - 0xFF80);
    if (ptr != 0)
        return (*ptr);
    return this->mbc_->read<T>(addr);
}

template<typename T>
void MMU::write(uint16_t addr, T value)
{
    T* ptr = 0;

    if (addr < 0x4000) // Bank 00
        this->mbc_->write<T>(addr, value);
    else if (0x4000 <= addr && addr < 0x8000) // Bank 01 .. NN
        this->mbc_->write<T>(addr, value);
    else if (0x8000 <= addr && addr < 0xA000) // VRAM
        ptr = (T*) (this->vram_ + addr - 0x8000);
    else if (0xA000 <= addr && addr < 0xC000) // RAM
        this->mbc_->write<T>(addr, value);
    else if (0xC000 <= addr && addr < 0xE000) // WRAM
        ptr = (T*) (this->wram_ + addr - 0xC000);
    else if (0xE000 <= addr && addr < 0xFE00) // ECHO
        ptr = (T*) (this->wram_ + addr - 0xE000);
    else if (0xFE00 <= addr && addr < 0xFEA0) // OAM
        ptr = (T*) (this->oam_ + addr - 0xFE00);
    else if (0xFEA0 <= addr && addr < 0xFF00) // Not usable
        return;
    else if (0xFF00 <= addr && addr < 0xFF80) // I/O Ports
        ptr = (T*) (this->io_ + addr - 0xFF00);
    else if (0xFF80 <= addr)
        ptr = (T*) (this->hram_ + addr - 0xFF80);
    if (ptr != 0)
        *ptr = value;
}

#endif // !MMU_HXX_
