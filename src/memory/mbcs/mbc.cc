#include "mbc.hh"

MBC::MBC(void* rom)
    : rom_bank_ (1), ram_bank_ (0)
{
    this->rom_ = (char*) rom;
    this->ram_ = (char*) malloc(0x20000 * sizeof (char));
}

MBC::~MBC()
{
    free(this->ram_);
}

void* MBC::read_address(uint16_t addr)
{
    // ROM Bank 00
    if (addr <= 0x3fff)
        return this->rom_ + addr;

    // ROM Bank 01 - 7F
    if (0x4000 <= addr && addr <= 0x7fff)
        return this->read_rom_address(addr);

    // RAM Bank 00 - 03
    if (0xa000 <= addr && addr <= 0xbfff)
        return this->read_ram_address(addr);

    // Other cases, we just return rom.
    return this->rom_ + addr;
}

void* MBC::write_address(uint16_t addr, uint16_t value)
{
    // RAM Enable
    if (addr <= 0x1fff)
        // used to protect the RAM during power-down, useless in an emulator
        return NULL;

    // ROM Bank Select, different for every controller
    if (0x2000 <= addr && addr <= 0x3fff)
        return this->write_rom_bank(addr, value);

    // RAM Bank Select, different for every controller
    if (0x4000 <= addr && addr <= 0x5fff)
        return this->write_ram_bank(addr, value);

    // Extra write
    if (0x6000 <= addr && addr <= 0x7fff)
        return this->write_extra_address(addr, value);

    // RAM Write
    if (0xa000 <= addr && addr <= 0xbfff)
        return this->write_ram_address(addr, value);

    return NULL;
}

// different for every controller
void* MBC::read_rom_address(uint16_t addr)
{
    return this->rom_ + addr - 0x4000 + 0x4000 * this->rom_bank_;
}

// different for every controller
void* MBC::read_ram_address(uint16_t addr)
{
    return this->ram_ + addr - 0xa000 + 0x2000 * this->ram_bank_;
}

void* MBC::write_rom_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    this->rom_bank_ = value;
    return NULL;
}

void* MBC::write_ram_bank(uint16_t addr, uint16_t value)
{
    (void)addr;

    this->ram_bank_ = value;
    return NULL;
}

// used only by MBC1 and MBC3
void* MBC::write_extra_address(uint16_t addr, uint16_t value)
{
    (void)addr;
    (void)value;

    return NULL;
}

// different for every controller
void* MBC::write_ram_address(uint16_t addr, uint16_t value)
{
    (void)value;

    return this->ram_ + addr - 0xa000 + this->ram_bank_ * 0x2000;
}
