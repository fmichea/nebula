#include "bgwtile.hh"

BGWTile::BGWTile(MMU& mmu, uint8_t x, uint8_t y, uint8_t data_select)
{
    uint16_t base = data_select ? 0x9C00 : 0x9800;
    uint8_t prev_vbk = mmu.read<uint8_t>(0xff4f);

    base += ((y % 256) / 8) * TILE_BY_LINE;
    base += (x % 256) / 8;

    mmu.write<uint8_t>(0xff4f, 0);
    uint8_t tile_number = mmu.read<uint8_t>(base);
    if (!mmu.LCDC.BGWTDS.get())
        tile_number -= 128;

    if (mmu.gb_type == GBType::CGB) {
        mmu.write<uint8_t>(0xff4f, 1);
        this->attribute = mmu.read<s_bgmap_attr>(base);
    }

    if (mmu.gb_type == GBType::CGB)
        mmu.write<uint8_t>(0xff4f, this->attribute.bank);
    base = mmu.LCDC.BGWTDS.get() ? 0x8000 : 0x8800;
    base += TILE_SZ * tile_number;
    base += ((mmu.gb_type == GBType::CGB && this->attribute.v_flip) ?
        7 - y % 8 : y % 8) * 2;

    uint16_t colors = mmu.read<uint16_t>(base);
    for (uint8_t it = 0; it < 8; ++it)
    {
        uint8_t shift = (mmu.gb_type == GBType::CGB && this->attribute.h_flip) ?
            it : 7 - it;
        this->colors_[it] = (colors >> shift) & 0x1;
        this->colors_[it] += 2 * ((colors >> (8 + shift)) & 0x1);
    }
    mmu.write<uint8_t>(0xff4f, prev_vbk);
}

uint8_t BGWTile::color(uint8_t x)
{
    return this->colors_[x % 8];
}
