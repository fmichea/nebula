#include "sprites.hh"

Sprite::Sprite(MMU* mmu, s_sprite& sprite, uint8_t y)
    : x_ (sprite.x - 8), y_ (sprite.y - 16)
{
    uint8_t tile_number = sprite.tile_number;
    uint8_t prev_vbk = mmu->read<uint8_t>(0xff4f);

    this->above_bg = sprite.flags.bg_to_obj ? false : true;

    if (mmu->gb_type == GBType::GB)
        this->palette = sprite.flags.bw_palette;
    else
        this->palette = sprite.flags.c_palette;

    // select correct VRAM bank
    if (mmu->gb_type == GBType::CGB)
        mmu->write<uint8_t>(0xff4f, sprite.flags.bank);

    if (mmu->LCDC.OBJSS.get()) tile_number &= ~1;
    uint16_t addr = 0x8000 + tile_number * 16;

    if (sprite.flags.y_flip)
        addr += 2 * ((SPRITE_HEIGHT(mmu) - (y - this->y_)));
    else
        addr += (y - this->y_) * 2;

    uint16_t colors = mmu->read<uint16_t>(addr);

    for (int it = 0; it < 8; ++it)
    {
        uint8_t shift = sprite.flags.x_flip ? it : 7 - it;
        this->line_[it] = (colors >> shift) & 0x1;
        this->line_[it] += 0x2 * ((colors >> (8 + shift)) & 0x1);
    }

    mmu->write<uint8_t>(0xff4f, prev_vbk);
}

uint8_t Sprite::color(uint8_t x) const
{
    return this->line_[x];
}

uint8_t Sprite::x_base() const
{
    return this->x_;
}

std::list<Sprite*> SpriteManager::get_sprites(MMU* mmu, uint8_t y)
{
    std::list<Sprite*> res;

    for (uint8_t it = 0; it < 40; ++it)
    {
        uint16_t addr = 0xFE00 + it * 4;
        s_sprite sprite = mmu->read<s_sprite>(addr);
        if (sprite.x == 0 || 168 <= sprite.x)
            continue;
        if (sprite.y == 0 || 160 <= sprite.y)
            continue;
        if (y < sprite.y - 16)
            continue;
        if (SPRITE_HEIGHT(mmu) <= y - (sprite.y - 16))
            continue;
        res.push_front(new Sprite(mmu, sprite, y));
    }
    return res;
}
