#ifndef MMU_HXX_
# define MMU_HXX_

# include "mmu.hh"

static const uint8_t _nr_masks[23] = {
    /*          NRx0 | NRx1 | NRx2 | NRx3 | NRx4 */
    /* NR1x */  0x80,  0x3F,  0x00,  0xFF,  0xBF,
    /* NR2x */  0xFF,  0x3F,  0x00,  0xFF,  0xBF,
    /* NR3x */  0x7F,  0xFF,  0x9F,  0xFF,  0xBF,
    /* NR4x */  0xFF,  0xFF,  0x00,  0x00,  0xBF,
    /* NR5x */  0x00,  0x00,  0x70
};

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
    if (ptr != 0) {
        T res = (*ptr);
        if (this->NR11.addr() <= addr && addr <= this->NR52.addr()) {
            // FIXME: quite dirty, any way to do this cleaner?
            uint8_t* tmp = (uint8_t*) &res;
            *tmp |= _nr_masks[addr - this->NR11.addr()];
        }
        return res;
    }
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
    else if (0xFF00 <= addr && addr < 0xFF80) { // I/O Ports
        if (addr == this->STAT.addr()) { // First three bits are read-only.
            //logging::info("stat: %x", value);
            ptr = (T*) (this->io_ + addr - 0xff00);
            *ptr = (*ptr & 0x07) | (value & 0xf8);
            return;
        }
        if (addr == this->DIV.addr() || addr == this->LY.addr())
            value = 0;
        ptr = (T*) (this->io_ + addr - 0xFF00);
    } else if (0xFF80 <= addr)
        ptr = (T*) (this->hram_ + addr - 0xFF80);

    // DMA Transfer and Start Address
    if (addr == 0xFF46)
    {
        for (uint8_t it = 0; it < 0xA0; ++it)
            this->oam_[it] = this->read<uint8_t>(((uint16_t) value << 8) | it);
        ptr = 0;
    }

    if (ptr != 0)
        *ptr = value;
}

template<> uint16_t MMU::read<uint16_t>(uint16_t addr);
template<> void MMU::write<uint16_t>(uint16_t addr, uint16_t value);

#endif // !MMU_HXX_
