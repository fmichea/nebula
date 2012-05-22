#include "sprites.hh"

Sprite::Sprite(MMU& mmu, s_sprite sprite, uint8_t y)
    : mmu_ (mmu), x_ (sprite.x), palette_ (sprite.flags.palette)
{
    this->above_bg_ = sprite.flags.bg_to_obj ? false : true;

    uint8_t tile_number = sprite.tile_number & (mmu.LCDC.OBJSS.get() ? 1 : 0);
    uint16_t addr = 0x8000 + tile_number * 2 * (mmu.LCDC.OBJSS.get() ? 8 : 16);

    if (sprite.flags.y_flip)
        addr += ((mmu.LCDC.OBJSS.get() ? 8 : 16) - (y - sprite.y)) * 2;
    else
        addr += (y - sprite.y) * 2 * 8;

    uint16_t colors = mmu.read<uint16_t>(addr);

    for (int it = 0; it < 7; ++it)
    {
        uint8_t shift = (sprite.flags.x_flip ? it : 7 - it);
        this->line_[it] = (colors >> (8 + shift)) + 0x2 * (colors >> shift);
    }
}

uint8_t Sprite::is_displayed(uint8_t x, uint8_t bkg_color) const
{
    return this->line_[x] && (this->above_bg_ || (line_[x] && !bkg_color));
}

uint8_t Sprite::color(uint8_t x) const
{
    return (this->mmu_.OBP[this->palette_].C[this->line_[x]].get());
}

uint8_t Sprite::x_base() const
{
    return this->x_;
}

std::list<Sprite> SpriteManager::get_sprites(MMU& mmu, uint8_t y)
{
    std::list<Sprite> res;

    for (uint8_t it = 0; it < 40; ++it)
    {
        uint16_t addr = 0xFE00 + it * 4;
        s_sprite sprite = mmu.read<s_sprite>(addr);
        if (sprite.x == 0 || 168 <= sprite.x)
            continue;
        if (sprite.y == 0 || 160 <= sprite.y)
            continue;
        if ((mmu.LCDC.OBJSS.get() ? 8 : 16) <= y - sprite.y)
            continue;
        res.push_back(Sprite(mmu, sprite, y));
    }
    return res;
}
