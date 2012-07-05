#include "bgwtile.hh"

BGWTile::BGWTile(MMU& mmu, uint8_t x, uint8_t y)
{
    uint16_t base = mmu.LCDC.BGTMDS.get() ? 0x9C00 : 0x9800;

    base += ((y % 256) / 8) * TILE_BY_LINE;
    base += (x % 256) / 8;

    print_debug("[-] ");
    uint8_t tile_number = mmu.read<uint8_t>(base);
    if (mmu.LCDC.BGWTDS.get())
        tile_number += 128;

    base = mmu.LCDC.BGWTDS.get() ? 0x8000 : 0x8800;
    base += TILE_SZ * tile_number + (y % 8) * 2;

    print_debug("[-] ");
    uint16_t colors = mmu.read<uint16_t>(base);
    print_debug("Colors found :");
    for (uint8_t it = 0; it < 8; ++it)
    {
        this->colors_[it] = (colors >> (15 - it)) & 0x1;
        this->colors_[it] += 2 * ((colors >> (7 - it)) & 0x1);
        print_debug(" %d", this->colors_[it]);
    }
    print_debug("\n");
}

uint8_t BGWTile::color(uint8_t x)
{
    return this->colors_[x % 8];
}
