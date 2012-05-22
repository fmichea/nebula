#ifndef SPRITES_HH_
# define SPRITES_HH_

# include <list>

# include "../memory/mmu.hh"

typedef struct
{
    uint8_t                 : 4;
    uint8_t     palette     : 1;
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

class Sprite
{
public:
    Sprite(MMU& mmu, s_sprite sprite, uint8_t y);

    uint8_t x_base() const;
    uint8_t is_displayed(uint8_t x, uint8_t bkg_color) const;
    uint8_t color(uint8_t x) const;

private:
    MMU&        mmu_;
    uint8_t     x_;
    uint8_t     palette_;
    bool        above_bg_;
    uint8_t     line_[8];
};

class SpriteManager
{
public:
    static std::list<Sprite> get_sprites(MMU& mmu, uint8_t y);
};

#endif // !SPRITES_HH_
