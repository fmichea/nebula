#ifndef NEBULA_GRAPHICS_SPRITES_HH_
# define NEBULA_GRAPHICS_SPRITES_HH_

# include <list>

# include "../memory/mmu.hh"

# define SPRITE_HEIGHT(mmu) (mmu->LCDC.OBJSS.get() ? 16 : 8)

typedef struct
{
    uint8_t     c_palette   : 3;
    uint8_t     bank        : 1;
    uint8_t     bw_palette  : 1;
    uint8_t     x_flip      : 1;
    uint8_t     y_flip      : 1;
    uint8_t     bg_to_obj   : 1;
} __attribute__ ((packed)) s_sprite_flags;

typedef struct
{
    uint8_t         y;
    uint8_t         x;
    uint8_t         tile_number;
    s_sprite_flags  flags;
} __attribute__ ((packed)) s_sprite;

class Sprite {
public:
    Sprite(MMU* mmu, s_sprite& sprite, uint8_t y);

    uint8_t x_base() const;
    uint8_t color(uint8_t x) const;

    uint8_t     palette;
    bool        above_bg;

private:
    int16_t     x_;
    int16_t     y_;
    uint8_t     line_[8];
};

class SpriteManager {
public:
    static std::list<Sprite*> get_sprites(MMU* mmu, uint8_t y);
};

#endif // !NEBULA_GRAPHICS_SPRITES_HH_
