#ifndef NEBULA_MEMORY_MMU_HXX_
# define NEBULA_MEMORY_MMU_HXX_

# include "mmu.hh"

static const uint8_t _nr_masks[32] = {
    /*          NRx0 | NRx1 | NRx2 | NRx3 | NRx4 */
    /* NR1x */  0x80,  0x3F,  0x00,  0xFF,  0xBF,
    /* NR2x */  0xFF,  0x3F,  0x00,  0xFF,  0xBF,
    /* NR3x */  0x7F,  0xFF,  0x9F,  0xFF,  0xBF,
    /* NR4x */  0xFF,  0xFF,  0x00,  0x00,  0xBF,
    /* NR5x */  0x00,  0x00,  0x70,
    /* WTF: */  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
                0xFF,  0xFF,  0xFF,  0xFF
};

template<typename T>
T MMU::read(uint16_t addr, bool twe) {
    T* ptr = 0;

    if (addr < 0x4000) { // Bank 00
        this->trigger_watch_event(twe, addr, WatchType::RO);
        return this->mbc_->read<T>(addr);
    } else if (0x4000 <= addr && addr < 0x8000) { // Bank 01 .. NN
        this->trigger_watch_event(twe, addr, WatchType::RO);
        return this->mbc_->read<T>(addr);
    } else if (0x8000 <= addr && addr < 0xA000) { // VRAM
        ptr = (T*) (this->vram_ + addr - 0x8000 + 0x2000 * this->VBK.get());
    } else if (0xA000 <= addr && addr < 0xC000) { // RAM
        this->trigger_watch_event(twe, addr, WatchType::RO);
        return this->mbc_->read<T>(addr);
    } else if (0xC000 <= addr && addr < 0xE000) { // WRAM
        if (this->gb_type == GBType::CGB && addr >= 0xD000
                && (this->SVBK.get() >= 2)) // banked WRAM
            ptr = (T*) (this->wram_ + (addr - 0xD000)
                + 0x1000 * this->SVBK.get());
        else
            ptr = (T*) (this->wram_ + addr - 0xC000);
    }
    else if (0xE000 <= addr && addr < 0xFE00) // ECHO
        ptr = (T*) (this->wram_ + addr - 0xE000);
    else if (0xFE00 <= addr && addr < 0xFEA0) // OAM
        ptr = (T*) (this->oam_ + addr - 0xFE00);
    //else if (0xFEA0 <= addr && addr < 0xFF00) // Not usable
        //return ((T) 0);
    else if (0xFF00 <= addr && addr < 0xFF80) { // I/O Ports
        ptr = (T*) (this->io_ + addr - 0xFF00);
        if (addr == this->BGPD.addr())
            ptr = (T*) (this->cbgp_ + (this->BGPI.get() & 0x3f));
        if (addr == this->OBPD.addr())
            ptr = (T*) (this->cobp_ + (this->OBPI.get() & 0x3f));
    }
    else if (0xFF80 <= addr)
        ptr = (T*) (this->hram_ + addr - 0xFF80);
    if (ptr != 0) {
        T res = (*ptr);
        if ((this->NR10.addr() <= addr && addr <= this->NR52.addr()) ||
            (0xFF27 <= addr && addr <= 0xFF2F)) {
            // FIXME: quite dirty, any way to do this cleaner?
            uint8_t* tmp = (uint8_t*) &res;
            *tmp |= _nr_masks[addr - this->NR10.addr()];
        }
        this->trigger_watch_event(twe, addr, WatchType::RO);
        return res;
    }
    this->trigger_watch_event(twe, addr, WatchType::RO);
    return this->mbc_->read<T>(addr);
}

template<typename T>
void MMU::write(uint16_t addr, T value, bool twe) {
    T* ptr = 0;

    if (addr < 0x4000) // Bank 00
        this->mbc_->write<T>(addr, value);
    else if (0x4000 <= addr && addr < 0x8000) // Bank 01 .. NN
        this->mbc_->write<T>(addr, value);
    else if (0x8000 <= addr && addr < 0xA000) // VRAM
        ptr = (T*) (this->vram_ + addr - 0x8000 + 0x2000 * this->VBK.get());
    else if (0xA000 <= addr && addr < 0xC000) // RAM
        this->mbc_->write<T>(addr, value);
    else if (0xC000 <= addr && addr < 0xE000) { // WRAM
        if (this->gb_type == GBType::CGB && addr >= 0xD000
                && (this->SVBK.get() >= 2)) // banked WRAM
            ptr = (T*) (this->wram_ + (addr - 0xD000)
                + 0x1000 * this->SVBK.get());
        else
            ptr = (T*) (this->wram_ + addr - 0xC000);
    }
    else if (0xE000 <= addr && addr < 0xFE00) // ECHO
        ptr = (T*) (this->wram_ + addr - 0xE000);
    else if (0xFE00 <= addr && addr < 0xFEA0) // OAM
        ptr = (T*) (this->oam_ + addr - 0xFE00);
    else if (0xFEA0 <= addr && addr < 0xFF00) // Not usable
        return;
    else if (0xFF00 <= addr && addr < 0xFF80) { // I/O Ports
        ptr = (T*) (this->io_ + addr - 0xFF00);
        if (addr == this->STAT.addr()) { // First three bits are read-only.
            //logging::info("stat: %x", value);
            *ptr = (*ptr & 0x07) | (value & 0xf8);
            this->trigger_watch_event(twe, addr, WatchType::WO);
            return;
        }
        if (addr == this->BGPD.addr()) {
            ptr = this->cbgp_ + (this->BGPI.get() & 0x3f);
            if (this->BGPI.get() >> 7) // auto-increment
                this->BGPI.set(
                    1 << 7 | (((this->BGPI.get() & 0x3f) + 1) & 0x3f));
        }
        if (addr == this->OBPD.addr()) {
            ptr = this->cobp_ + (this->OBPI.get() & 0x3f);
            if (this->OBPI.get() >> 7) // auto-increment
                this->OBPI.set(
                    1 << 7 | (((this->OBPI.get() & 0x3f) + 1) & 0x3f));
        }
        if (addr == this->VBK.addr())
            value &= this->gb_type == GBType::CGB ? 0x01 : 0x00;
        if (addr == this->SVBK.addr())
            value &= this->gb_type == GBType::CGB ? 0x07 : 0x00;
        if (addr == this->KEY1.addr())
            value = (value & 0x1) | (this->KEY1.get() & 0x80);
        if (addr == this->UNDOC6C.addr())
            value |= 0xfe;
        if (addr == this->UNDOC75.addr())
            value |= 0x8f; // why, why, why...
        if (addr == this->UNDOC76.addr() || addr == this->UNDOC77.addr()) // RO
            return;
        if (addr == this->DIV.addr() || addr == this->LY.addr())
            value = 0;
        if (addr == this->NR52.addr()) {
            //logging::info("nr52 = %x (%x)", this->NR52.get(), value);
            this->NR52.sound_on.set((value >> 7) & 1);
            //logging::info("nr52 = %x", this->NR52.get());
            if (!this->NR52.sound_on.get()) {
                for (int it = 0xFF10; it < 0xFF26; ++it)
                    this->io_[it - 0xFF00] = 0;
            }
            ptr = nullptr;
        }
        if ((this->NR10.addr() <= addr && addr <= this->NR52.addr())) {
            //logging::info("Trying to write %x at %x. (%s)", value, addr,
                          //(this->NR52.sound_on.get() ? "ON" : "OFF"));
            if (!this->NR52.sound_on.get()) {
                //logging::info("Ignore write %x...", addr);
                ptr = nullptr;
            }
        }
    } else if (0xFF80 <= addr)
        ptr = (T*) (this->hram_ + addr - 0xFF80);

    // DMA Transfer and Start Address
    if (addr == 0xFF46) {
        for (uint8_t it = 0; it < 0xA0; ++it)
            this->oam_[it] = this->read<uint8_t>(((uint16_t) value << 8) | it);
        ptr = 0;
    }
    // HDMA start
    if (this->gb_type == GBType::CGB && addr == this->HDMA5.addr()) {
        // stop current HDMA
        if (this->hdma_active && (this->HDMA5.get() >> 7) == 0) {
            this->hdma_active = false;
            this->HDMA5.set(0xff);
        }
        else {
            uint16_t srcaddr = (this->HDMA1.get() << 8) | this->HDMA2.get();
            uint16_t dstaddr = (this->HDMA3.get() << 8) | this->HDMA4.get();
            uint16_t length = ((this->HDMA5.get() & 0x7f) + 1) * 0x10;
            srcaddr &= 0xfff0;
            dstaddr = (dstaddr & 0x1ff0) | 0x8000; // ensure we are in VRAM
            this->hdma_active = false;
            // invalid address
            if (!((srcaddr >= 0x0000 && srcaddr <= 0x7ff0)
                    || (srcaddr >= 0xa000 && srcaddr <= 0xdf00)))
                return;
            // start general purpose DMA
            if ((this->HDMA5.get() >> 7) == 0) {
                while (length > 0) {
                    this->write<uint8_t>(dstaddr++,
                        this->read<uint8_t>(srcaddr++));
                    --length;
                }
                value = 0xff;
            }
            // start H-blank DMA
            else {
                this->hdma_active = true;
                this->hdma_index_ = 0;
                this->hdma_length_ = length;
                value = 0x00;
            }
        }
    }
    if (ptr != 0) {
        *ptr = value;
        this->trigger_watch_event(twe, addr, WatchType::WO);
    }
}

template<> uint16_t MMU::read<uint16_t>(uint16_t addr, bool twe);
template<> void MMU::write<uint16_t>(uint16_t addr, uint16_t value, bool twe);

#endif // !NEBULA_MEMORY_MMU_HXX_
