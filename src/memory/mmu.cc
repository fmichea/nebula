#include "mmu.hh"

bool MMU::load_rom(std::string filename)
{
    struct stat stat;

    // Opening and mapping file.
    this->fd_ = open(filename.c_str(), O_RDONLY);
    if (this->fd_ < 0)
        return false;
    fstat(this->fd_, &stat);
    this->size_ = stat.st_size < 0x10000 ? 0x10000 : stat.st_size;
    this->rom_ = (char*) mmap(0, this->size_, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE, this->fd_, 0);
    if (this->rom_ == MAP_FAILED)
        return false;

    // Checking that ROM was correctly loaded.
    const char* nintendo_logo =
        "\xce\xed\x66\x66\xcc\x0d\x00\x0b\x03\x73\x00\x83\x00\x0c\x00\x0d"
        "\x00\x08\x11\x1f\x88\x89\x00\x0e\xdc\xcc\x6e\xe6\xdd\xdd\xd9\x99"
        "\xbb\xbb\x67\x63\x6e\x0e\xec\xcc\xdd\xdc\x99\x9f\xbb\xb9\x33\x3e";
    if (memcmp(nintendo_logo, this->rom_ + 0x104, 0x30))
        return false;

    // ROM Title
    char title[0x11];
    strncpy(title, this->rom_ + 0x134, 0x10);
    this->title_ = title;

    // Cartridge Type
    if (!this->load_cartridge_type(this->rom_[0x147]))
        return false;

    // MBC Type
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

    // Header checksum
    uint8_t x = 0;
    for (int it = 0x134; it < 0x14d; ++it)
    {
        x = x - this->rom_[it] - 1;
    }
    if (this->rom_[0x14d] != x)
        return false;

    // Global checksum.
    // Not done: GameBoy doesn't do it either...

	// Reset all registers.
	this->reset_registers();
    return true;
}

bool MMU::load_cartridge_type(uint8_t val)
{
    // FIXME
    (void) val;
    return true;
}

bool MMU::load_rom_size(uint8_t val)
{
    // FIXME
    (void) val;
    return true;
}

bool MMU::load_ram_size(uint8_t val)
{
    // FIXME
    (void) val;
    return true;
}

bool MMU::load_mbc(uint8_t val)
{
    switch (val)
    {
    case 0x00:
        this->mbc_ = new ROMOnly(this->rom_);
        return true;
    case 0x01:
    case 0x02:
    case 0x03:
        this->mbc_ = new MBC1(this->rom_);
        return true;
    };
    return false;
}

#define R(Reg, Addr, Value)                                     \
    this->Reg = Register((uint8_t&) this->rom_[Addr]);          \
    this->Reg = Value;

bool MMU::reset_registers()
{
    R(IE, 0xffff, 0x00)
    R(LY, 0xff44, 0x00)
    R(LYC, 0xff45, 0x00)
    R(NR10, 0xff10, 0x80)
    R(NR11, 0xff11, 0xbf)
    R(NR12, 0xff12, 0xf3)
    R(NR14, 0xff14, 0xbf)
    R(NR21, 0xff16, 0x3f)
    R(NR22, 0xff17, 0x00)
    R(NR24, 0xff19, 0xbf)
    R(NR30, 0xff1a, 0x7f)
    R(NR31, 0xff1b, 0xff)
    R(NR32, 0xff1c, 0x9f)
    R(NR33, 0xff1e, 0xbf)
    R(NR41, 0xff20, 0xff)
    R(NR42, 0xff21, 0x00)
    R(NR43, 0xff22, 0x00)
    R(NR44, 0xff23, 0xbf)
    R(NR50, 0xff24, 0x77)
    R(NR51, 0xff25, 0xf3)
    R(NR52, 0xff26, 0xf1)
    R(SCX, 0xff43, 0x00)
    R(SCY, 0xff42, 0x00)
    R(TAC, 0xff07, 0x00)
    R(TIMA, 0xff05, 0x00)
    R(TMA, 0xff06, 0x00)
    R(WX, 0xff4b, 0x00)
    R(WY, 0xff4a, 0x00)
    return true;
}
