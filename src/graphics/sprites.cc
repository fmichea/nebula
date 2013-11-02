#include "sprites.hh"

Sprite::Sprite(MMU& mmu, s_sprite& sprite, uint8_t y)
    : mmu_ (mmu), x_ (sprite.x - 8), y_ (sprite.y - 16),
      palette_ (sprite.flags.palette)
{
    uint8_t tile_number = sprite.tile_number;

    this->above_bg_ = sprite.flags.bg_to_obj ? false : true;

    if (mmu.LCDC.OBJSS.get()) tile_number &= ~1;
    uint16_t addr = 0x8000 + tile_number * 16;

    if (sprite.flags.y_flip)
        addr += 2 * ((SPRITE_HEIGHT - (y - this->y_)));
    else
        addr += (y - this->y_) * 2;

    uint16_t colors = mmu.read<uint16_t>(addr);

    for (int it = 0; it < 8; ++it)
    {
        uint8_t shift = (sprite.flags.x_flip ? it : 7 - it);
        this->line_[it] = (colors >> shift) & 0x1;
        this->line_[it] += 0x2 * ((colors >> (8 + shift)) & 0x1);
    }
}

uint8_t Sprite::is_displayed(uint8_t x, uint8_t bkg_color) const
{
    return (this->line_[x] && (this->above_bg_ || !bkg_color));
}

uint8_t Sprite::color(uint8_t x) const
{
    PaletteProxy& palette = this->mmu_.OBP1;
    if (this->palette_ == 1) {
        palette = this->mmu_.OBP2;
    }
    return palette.C[this->line_[x]].get();
}

uint8_t Sprite::x_base() const
{
    return this->x_;
}

std::list<Sprite*> SpriteManager::get_sprites(MMU& mmu, uint8_t y)
{
    std::list<Sprite*> res;

    for (uint8_t it = 0; it < 40; ++it)
    {
        uint16_t addr = 0xFE00 + it * 4;
        s_sprite sprite = mmu.read<s_sprite>(addr);
        if (sprite.x == 0 || 168 <= sprite.x)
            continue;
        if (sprite.y == 0 || 160 <= sprite.y)
            continue;
        if (y < sprite.y - 16)
            continue;
        if (SPRITE_HEIGHT <= y - (sprite.y - 16))
            continue;
        res.push_back(new Sprite(mmu, sprite, y));
    }
    return res;
}
