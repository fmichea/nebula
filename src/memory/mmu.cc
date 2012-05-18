#include "mmu.hh"

MMU::MMU()
{}

MMU::~MMU()
{
    if (this->rom_ != 0)
        munmap(this->rom_, this->size_);
    if (this->mbc_ != 0)
        delete this->mbc_;
}

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
    {
        print_debug("Nintendo logo doesn't match!\n");
        return false;
    }

    // ROM Title
    char title[0x11];
    strncpy(title, this->rom_ + 0x134, 0x10);
    this->title_ = title;
    print_debug("ROM Title: %s.\n", title);

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
    print_debug("Target: %s\n", this->target_.c_str());

    // Header checksum
    int8_t x = 0;
    for (int it = 0x134; it < 0x14d; ++it)
    {
        x = x - this->rom_[it] - 1;
    }
    if (this->rom_[0x14d] != x)
    {
        print_debug("Cartridge header checksum failure: %x (expected %x).\n",
                    x, this->rom_[0x14d]);
        return false;
    }

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

bool MMU::reset_registers()
{
#define X(Reg, Addr, Value)                                     \
    this->Reg = Register((uint8_t*) (this->rom_ + Addr));      \
    this->Reg = Value;
#include "registers.hh"
#undef X
    return true;
}
