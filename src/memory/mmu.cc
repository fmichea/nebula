#include "mmu.hh"

MMU::MMU()
    : stopped(false), fd_ (0), mbc_ (0)
{
    this->rom_ = 0;
    this->vram_ = new uint8_t[0x2000];
    memset(this->vram_, 0, 0x2000);
    this->wram_ = new uint8_t[0x8000];
    this->oam_ = new uint8_t[0xA0];
    this->io_ = new uint8_t[0x80];
    this->hram_ = new uint8_t[0x80];
}

MMU::~MMU()
{
    if (this->rom_ != 0)
        free(this->rom_);
    if (this->mbc_ != 0)
        delete this->mbc_;
    if (this->fd_ != 0)
        close(this->fd_);
    delete[] this->vram_;
    delete[] this->wram_;
    delete[] this->oam_;
    delete[] this->io_;
    delete[] this->hram_;
}

bool MMU::load_rom(std::string filename)
{
    struct stat stat;

    // Opening and mapping file.
    this->fd_ = open(filename.c_str(), O_RDONLY);
    if (this->fd_ < 0)
        return false;
    fstat(this->fd_, &stat);
    this->size_ = stat.st_size;
    char* mapped = (char*) mmap(0, stat.st_size, PROT_READ, MAP_PRIVATE,
                                this->fd_, 0);
    this->rom_ = (uint8_t*) malloc(this->size_ * sizeof (uint8_t));
    if (this->rom_ == MAP_FAILED)
        return false;
    memcpy(this->rom_, mapped, stat.st_size);
    munmap(mapped, stat.st_size);

    // Checking that ROM was correctly loaded.
    const char* nintendo_logo =
        "\xce\xed\x66\x66\xcc\x0d\x00\x0b\x03\x73\x00\x83\x00\x0c\x00\x0d"
        "\x00\x08\x11\x1f\x88\x89\x00\x0e\xdc\xcc\x6e\xe6\xdd\xdd\xd9\x99"
        "\xbb\xbb\x67\x63\x6e\x0e\xec\xcc\xdd\xdc\x99\x9f\xbb\xb9\x33\x3e";
    if (memcmp(nintendo_logo, this->rom_ + 0x104, 0x30))
    {
        logging::warning("Nintendo logo doesn't match!");
        return false;
    }

    // ROM Title
    strncpy(this->title_, (const char*) (this->rom_ + 0x134), 0x10);
    logging::info("ROM Title: %s.", this->title_);

    // Cartridge Type
    if (!this->load_mbc(this->rom_[0x147]))
        return false;

    // ROM Size
    if (!this->load_rom_size(this->rom_[0x148]))
        return false;

    // RAM Size
    if (!this->load_ram_size(this->rom_[0x149]))
        return false;

    // Target country
    this->target_ = this->rom_[0x14a] ? "Non-Japanese" : "Japanese";
    logging::info("Target: %s", this->target_.c_str());

    // Header checksum
    uint8_t x = 0;
    for (int it = 0x134; it < 0x14d; ++it)
    {
        x = x - this->rom_[it] - 1;
    }
    if (this->rom_[0x14d] != x)
    {
        logging::error("Cartridge header checksum failure: %x (expected %x).",
                       x, this->rom_[0x14d]);
        return false;
    }

    // Global checksum.
    // Not done: GameBoy doesn't do it either...

    // Reset all registers.
    this->reset_registers();
    return true;
}

bool MMU::load_rom_size(uint8_t val)
{
    // FIXME
    (void) val;
    logging::info("ROM Size: %x.", val);
    return true;
}

bool MMU::load_ram_size(uint8_t val)
{
    // FIXME
    (void) val;
    logging::info("RAM Size: %x.", val);
    return true;
}

bool MMU::load_mbc(uint8_t val)
{
    std::string name;

    // MBC Setting.
    switch (val)
    {
    case 0x00:
        this->mbc_ = new ROMOnly(this->rom_);
        break;
    case 0x01:
    case 0x02:
    case 0x03:
        this->mbc_ = new MBC1(this->rom_);
        break;
    };

    // Name
    switch (val)
    {
    case 0x00:
        name = "ROM Only";
        break;
    case 0x01:
        name = "MBC1";
        break;
    case 0x02:
        name = "MBC1+RAM";
        break;
    case 0x03:
        name = "MBC1+RAM+BATTERY";
        break;
    };
    if (name.size() != 0)
        logging::info("Memory type: %s", name.c_str());
    return (this->mbc_ != 0);
}

bool MMU::reset_registers()
{
#define X(Reg, Addr, Value)                                     \
    this->Reg = Register(this->io_ + Addr - 0xff00, Addr);      \
    this->Reg.set(Value);
#include "registers.hh"
#undef X
    this->LCDC = LCDCProxy(this->io_ + 0xff40 - 0xff00);
    this->io_[0xff40 - 0xff00] = 0x91;
    this->STAT = STATProxy(this->io_ + 0xff41 - 0xff00);
    this->STAT.mode.set(0);
    this->BGP = PaletteProxy(this->io_ + 0xff47 - 0xff00);
    this->io_[0xff47 - 0xff00] = 0xFC;
    this->OBP[0] = PaletteProxy(this->io_ + 0xff48 - 0xff00);
    this->io_[0xff48 - 0xff00] = 0xFF;
    this->OBP[1] = PaletteProxy(this->io_ + 0xff49 - 0xff00);
    this->io_[0xff49 - 0xff00] = 0xFF;
    this->IE = Register(this->hram_ + 0xffff - 0xff80, 0xffff);
    this->IE.set(0x00);
    this->write<uint8_t>(0xFF00, 0x3f);
    return true;
}

template<>
uint16_t MMU::read<uint16_t>(uint16_t addr) {
    uint16_t res = this->read<uint8_t>(addr);
    res |= ((uint16_t) this->read<uint8_t>(addr + 1)) << 8;
    return res;
}

template<>
void MMU::write<uint16_t>(uint16_t addr, uint16_t value) {
    this->write<uint8_t>(addr, value & 0xff);
    this->write<uint8_t>(addr + 1, (value >> 8) & 0xff);
}
