#include "mmu.hh"

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

MMU::MMU()
    : stopped (false)
    , _watch ()
    , fd_ (0)
    , mbc_ (0)
{
#define X(Type, Name, Size, InitVal)    \
    this->Name = new Type[Size];        \
    memset(this->Name, InitVal, Size);
#include "mmaps.def"
#undef X
}

MMU::~MMU() {
    delete this->mbc_;
    if (this->fd_ != 0) {
        close(this->fd_);
    }

#define X(Type, Name, Size, InitVal)  \
    delete[] this->Name;
#include "mmaps.def"
#undef X
}

void MMU::subscribe(uint16_t addr, WatchType type, WatchHook hook, void* data) {
    this->_watch.subscribe(addr, type, hook, data);
}

void MMU::unsubscribe(uint16_t addr, WatchHook hook) {
    this->_watch.unsubscribe(addr, hook);
}

namespace nms = nebula::memory::segments;

nms::Segment* MMU::_get_segment(uint16_t addr) {
    // Segments provided by the GameBoy directly.
    for (nms::Segment* seg : nms::SEGMENTS) {
        if (seg->contains_address(addr)) {
            return seg;
        }
    }
    return nullptr;
}

/**
 * This function returns a pointer to the real memory address to read from
 * based on a "virtual" GameBoy address, or null.
 *
 * It is always safe to read a byte if pointer is not null. For any other size,
 * it is expected that the read will not be across multiple zones. This
 * assumption is not checked.
 */
uint8_t* MMU::_real_byte_ptr(AccessType type, uint16_t addr, uint8_t value) {
    // Special GameBoy Color registers.
    if (addr == this->BGPD.addr()) {
        return this->cbgp_ + (this->BGPI.get() & 0x3f);
    } else if (addr == this->OBPD.addr()) {
        return this->cobp_ + (this->OBPI.get() & 0x3f);
    }

    nms::Segment* seg = this->_get_segment(addr);

    if (seg == nullptr) {
        return nullptr;
    }

    if (seg->mbc() && this->mbc_ != nullptr) {
        return this->mbc_->real_byte_ptr(type, addr, value);
    }
    return seg->ptr(addr);
}

uint8_t MMU::_read_byte_masking(uint16_t addr, uint8_t value) {
    if (this->NR10.addr() <= addr && addr <= 0xFF2F) {
        value |= _nr_masks[addr - this->NR10.addr()];
    }
    return value;
}

uint8_t MMU::_read_byte(uint16_t addr) {
    uint8_t* ptr = this->_real_byte_ptr(AccessType::READ, addr, 0);

    if (ptr != 0) {
        uint8_t res = this->_read_byte_masking(addr, *ptr);
        this->_watch.create_event(AccessType::READ, addr)->set_values(0, res);
        return res;
    }

    // FIXME: If we get here, something bad happened.
    return 0;
}

uint8_t* MMU::_write_byte_ptr(uint16_t addr, uint8_t value) {
    // All of these registers are read-only.
    if (nms::IO_PORTS.contains_address(addr)) {
        if (addr == this->UNDOC76.addr()) {
            return nullptr;
        }

        if (addr == this->UNDOC77.addr()) {
            return nullptr;
        }

        if (addr == this->NR52.addr()) {
            return nullptr;
        }

        if ((this->NR10.addr() <= addr && addr < this->NR52.addr())) {
#if 0
            logging::info("Trying to write %x at %x. (%s)", value, addr,
                          (this->NR52.sound_on.get() ? "ON" : "OFF"));
#endif
            if (!this->NR52.sound_on.get()) {
                //logging::info("Ignore write %x...", addr);
                return nullptr;
            }
        }
    }

    if (addr == this->DMA.addr()) {
        uint16_t srcaddr = (uint16_t) value << 8;
        this->memcpy(nms::OAM.buffer(), srcaddr, nms::OAM.size());
        return nullptr;
    }

    if (addr == this->NR52.addr()) {
        logging::info("nr52 = %x (%x)", this->NR52.get(), value);
        uint8_t sound_on = (value >> 7) & 1;
        if (!sound_on) {
            this->NR52.sound_on.set(1);
            for (int it = 0xFF10; it <= 0xFF26; ++it) {
                this->write<uint8_t>(it, 0);
            }
        }
        this->NR52.sound_on.set(sound_on);
        logging::info("nr52 = %x", this->NR52.get());
        return nullptr;
    }

    return this->_real_byte_ptr(AccessType::WRITE, addr, value);
}

uint8_t MMU::_write_byte_masking(uint16_t addr, uint8_t* ptr, uint8_t value) {
    if (nms::IO_PORTS.contains_address(addr)) {
        if (addr == this->STAT.addr()) { // First three bits are read-only.
            value = (*ptr & 0x07) | (value & 0xf8);
        } else if (addr == this->BGPD.addr() && this->BGPI.get() >> 7) { // auto-increment
            uint8_t tmp = ((this->BGPI.get() & 0x3f) + 1) & 0x3f;
            this->BGPI.set(1 << 7 | tmp);
        } else if (addr == this->OBPD.addr() && this->OBPI.get() >> 7) { // auto-increment
            uint8_t tmp = ((this->OBPI.get() & 0x3f) + 1) & 0x3f;
            this->OBPI.set(1 << 7 | tmp);
        } else if (addr == this->VBK.addr()) {
            value &= this->gb_type == GBType::CGB ? 0x01 : 0x00;
            nms::VRAM.select_bank(value);
        } else if (addr == this->SVBK.addr()) {
            value &= this->gb_type == GBType::CGB ? 0x07 : 0x00;
            if (this->gb_type == GBType::CGB && 1 < value) {
                nms::WRAM.select_bank(value - 1);
            }
        } else if (addr == this->KEY1.addr()) {
            value = (value & 0x1) | (this->KEY1.get() & 0x80);
        } else if (addr == this->UNDOC6C.addr()) {
            value |= 0xfe;
        } else if (addr == this->UNDOC75.addr()) {
            value |= 0x8f; // why, why, why...
        } else if (addr == this->DIV.addr() || addr == this->LY.addr()) {
            value = 0;
        }
    }

    // HDMA start
    if (this->gb_type == GBType::CGB && addr == this->HDMA5.addr()) {
        // stop current HDMA
        if (this->hdma_active && (this->HDMA5.get() >> 7) == 0) {
            this->hdma_active = false;
            this->HDMA5.set(0xff);
        } else {
            uint16_t srcaddr = (this->HDMA1.get() << 8) | this->HDMA2.get();
            uint16_t dstaddr = (this->HDMA3.get() << 8) | this->HDMA4.get();
            uint16_t length = ((this->HDMA5.get() & 0x7f) + 1) * 0x10;

            srcaddr &= 0xfff0;
            dstaddr = (dstaddr & 0x1ff0) | 0x8000; // ensure we are in VRAM

            this->hdma_active = false;
            // invalid address
            if (!((srcaddr >= 0x0000 && srcaddr <= 0x7ff0)
                    || (srcaddr >= 0xa000 && srcaddr <= 0xdf00)))
                return value;
            // start general purpose DMA
            if ((this->HDMA5.get() >> 7) == 0) {
                while (length > 0) {
                    this->_write_byte(dstaddr++, this->_read_byte(srcaddr++));
                    --length;
                }
                value = 0xff;
            } else { // start H-blank DMA
                this->hdma_active = true;
                this->hdma_index_ = 0;
                this->hdma_length_ = length;
                value = 0x00;
            }
        }
    }

    return value;
}

void MMU::_write_byte(uint16_t addr, uint8_t value) {
    uint8_t* ptr = this->_write_byte_ptr(addr, value);

    if (ptr != 0) {
        uint8_t old_val = this->_read_byte_masking(addr, *ptr);

        value = this->_write_byte_masking(addr, ptr, value);
        this->_watch.create_event(AccessType::WRITE, addr)->set_values(old_val, value);
        *ptr = value;
    }
}

bool MMU::memcpy(uint8_t* dst, uint16_t srcaddr, uint16_t size) {
    nms::Segment* seg = this->_get_segment(srcaddr);

    if (seg == nullptr) {
        return false;
    }

    if (!seg->contains_address(srcaddr + size - 1)) {
        return false;
    }

    std::memcpy(dst, seg->ptr(srcaddr), size);
    return true;
}

bool MMU::load_rom(std::string filename) {
    struct stat stat;

    // Reset all registers.
    this->reset_registers();

    // Opening and mapping file.
    this->fd_ = open(filename.c_str(), O_RDONLY);
    if (this->fd_ < 0)
        return false;
    fstat(this->fd_, &stat);
    this->size_ = stat.st_size;

    char* mapped = (char*) mmap(0, stat.st_size, PROT_READ, MAP_PRIVATE, this->fd_, 0);
    if (mapped == MAP_FAILED) {
        logging::error("ROM mapping failed.");
        return false;
    }

    // ROM Banks are all stored in ROM.
    std::memcpy(nms::ROM.buffer(), mapped, stat.st_size);

    // Do not need the mapped file anymore.
    munmap(mapped, stat.st_size);

    // ROM type
    uint8_t cgb_flag = this->read<uint8_t>(0x143);
    if (cgb_flag == 0x80 || cgb_flag == 0xC0) {
        this->gb_type = GBType::CGB;
    } else {
        this->gb_type = GBType::GB;
    }

    // Checking that ROM was correctly loaded.
    char nl[NINTENDO_LOGO_SIZE];

    if (!this->memcpy((uint8_t*) nl, 0x104, NINTENDO_LOGO_SIZE)) {
        logging::error("Failed to copy nintendo logo from ROM.");
        return false;
    }

    if (std::memcmp(NINTENDO_LOGO, nl, this->gb_type == GBType::CGB ? 0x18 : NINTENDO_LOGO_SIZE)) {
        logging::error("Nintendo logo doesn't match!");
        return false;
    }

    // ROM Title
    uint8_t title_length = this->gb_type == GBType::CGB ? 15 : 16;
    if (!this->memcpy((uint8_t*) this->title_, 0x134, title_length)) {
        logging::error("Failed to copy cartridge title.");
        return false;
    }
    logging::info("ROM Title: %s.", this->title_);

    // Cartridge Type
    if (!this->load_mbc(this->read<uint8_t>(0x147))) {
        return false;
    }

    // ROM Size
    if (!this->load_rom_size(this->read<uint8_t>(0x148))) {
        return false;
    }

    // RAM Size
    if (!this->load_ram_size(this->read<uint8_t>(0x149))) {
        return false;
    }

    // Target country
    this->target_ = this->read<uint8_t>(0x14a) ? "Non-Japanese" : "Japanese";
    logging::info("Target: %s", this->target_.c_str());

    // Header checksum
    uint8_t checksum = 0;
    for (int it = 0x134; it <= 0x14d; ++it) {
        checksum -= (this->read<uint8_t>(it) + 1);
    }
    if (checksum != 0xff) {
        logging::error("Cartridge header checksum failure.");
        return false;
    }

    // Global checksum.
    // Not done: GameBoy doesn't do it either...

    return true;
}

bool MMU::load_rom_size(uint8_t val) {
    // FIXME
    logging::info("ROM Size: %x.", val);
    return true;
}

bool MMU::load_ram_size(uint8_t val)
{
    // FIXME
    logging::info("RAM Size: %x.", val);
    return true;
}

bool MMU::load_mbc(uint8_t val)
{
    const char* names[0x1F] = {
        "ROM ONLY", /* 0x00 */
        "MBC1", "MBC1+RAM", "MBC1+RAM+BATTERY", /* 0x01 -> 0x03 */
        nullptr,
        "MBC2", "MBC2+BATTERY", /* 0x05 -> 0x06 */
        nullptr,
        "ROM+RAM", "ROM+RAM+BATTERY", /* 0x08 -> 0x09 */
        nullptr,
        "MMM01", "MMM01+RAM", "MMM01+RAM+BATTERY", /* 0x0B -> 0x0D */
        nullptr,
        "MBC3+TIMER+BATTERY", "MBC3+TIMER+RAM+BATTERY", "MBC3", "MBC3+RAM",
        "MBC3+RAM+BATTERY", /* 0x0F -> 0x13 */
        nullptr,
        "MBC4", "MBC4+RAM", "MBC4+RAM+BATTERY", /* 0x15 -> 0x17 */
        nullptr,
        "MBC5", "MBC5+RAM", "MBC5+RAM+BATTERY", "MBC5+RUMBLE+RAM",
        "MBC5+RUMBLE+RAM+BATTERY" /* 0x19 -> 0x1E */
    };

    // MBC Setting.
    switch (val)
    {
    case 0x00:
        this->mbc_ = new ROMOnly();
        break;
    case 0x01:
    case 0x02:
    case 0x03:
        this->mbc_ = new MBC1();
        break;
    case 0x05:
    case 0x06:
        this->mbc_ = new MBC2();
        break;
    case 0x0F:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
        this->mbc_ = new MBC3();
        break;
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
        this->mbc_ = new MBC5();
        break;
    };

    // Name
    const char* name = nullptr;
    if (val <= 0x1E)
        name = names[val];
    else {
        switch (val) {
        case 0xFC:
            name = "POCKET CAMERA";
            break;
        case 0xFD:
            name = "BANDAI TAMA5";
            break;
        case 0xFE:
            name = "HuC3";
            break;
        case 0xFF:
            name = "HuC1+RAM+BATTERY";
            break;
        };
    }
    if (name != 0)
        logging::info("Cartridge type: %s", name);
    else
        logging::error("Cartridge type is not a known value.");

    if (this->mbc_ == nullptr)
        logging::error("This cartridge type was not implemented.");

    return (name != nullptr && this->mbc_ != nullptr);
}

bool MMU::reset_registers()
{
    this->IE = RegisterProxy(nms::HRAM.ptr(0xffff), 0xffff);
    this->IE.set(0x00);
#define X2(RegType, Reg, Addr, Value)                           \
    this->Reg = RegType(nms::IO_PORTS.ptr(Addr), Addr);     \
    this->Reg.set(Value);
#define X1(Reg, Addr, Value) X2(RegisterProxy, Reg, Addr, Value)
#include "registers.def"
#undef X1
#undef X2
    return true;
}

void MMU::do_hdma() {
    if (this->hdma_index_ < this->hdma_length_) {
        uint16_t srcaddr = (this->HDMA1.get() << 8) | this->HDMA2.get();
        uint16_t dstaddr = (this->HDMA3.get() << 8) | this->HDMA4.get();

        srcaddr &= 0xfff0;
        dstaddr = (dstaddr & 0x1ff0) | 0x8000; // ensure we are in VRAM

        for (size_t i = 0; i < 0x10; ++i) {
            uint16_t dstaddr_i = dstaddr + this->hdma_index_ + i;
            uint16_t srcaddr_i = srcaddr + this->hdma_index_ + i;

            this->write<uint8_t>(dstaddr_i, this->read<uint8_t>(srcaddr_i));
        }

        this->hdma_index_ += 0x10;
        this->HDMA5.set((this->hdma_length_ - this->hdma_index_) / 0x10 - 1);
    } else { // HDMA finished
        this->hdma_active = false;
        this->HDMA5.set(0xff);
    }
}

template<>
uint8_t MMU::read<uint8_t>(uint16_t addr) {
    uint8_t res = this->_read_byte(addr);
    this->_watch.trigger_all_events();
    return res;
}

template<>
uint16_t MMU::read<uint16_t>(uint16_t addr) {
    uint16_t result;

    result = static_cast<uint16_t>(this->_read_byte(addr + 1)) << 8;
    result |= this->_read_byte(addr);

    this->_watch.trigger_all_events();
    return result;
}

template<>
void MMU::write<uint8_t>(uint16_t addr, uint8_t value) {
    this->_write_byte(addr, value);
    this->_watch.trigger_all_events();
}

template<>
void MMU::write<uint16_t>(uint16_t addr, uint16_t value) {
    this->_write_byte(addr, value & 0xff);
    this->_write_byte(addr + 1, (value >> 8) & 0xff);
    this->_watch.trigger_all_events();
}
