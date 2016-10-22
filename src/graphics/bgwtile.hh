#ifndef NEBULA_GRAPHICS_BGWTILE_HH_
# define NEBULA_GRAPHICS_BGWTILE_HH_

# include "../memory/mmu.hh"

# define TILE_BY_LINE 32
# define TILE_SZ 16

typedef struct {
    uint8_t palette     : 3;
    uint8_t bank        : 1;
    uint8_t             : 1;
    uint8_t h_flip      : 1;
    uint8_t v_flip      : 1;
    uint8_t priority    : 1;
} __attribute__ ((packed)) s_bgmap_attr;

class BGWTile
{
public:
    BGWTile(MMU* mmu, uint8_t x, uint8_t y, uint8_t data_select);

    uint8_t color(uint8_t x);

    s_bgmap_attr attribute;

private:
    uint8_t colors_[8];
};

#endif // !NEBULA_GRAPHICS_BGWTILE_HH_
