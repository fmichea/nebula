#ifndef BGTILE_HH_
# define BGTILE_HH_

# include "../memory/mmu.hh"

# define TILE_BY_LINE 32
# define TILE_SZ 16

class BGWTile
{
public:
    BGWTile(MMU& mmu, uint8_t x, uint8_t y);

    uint8_t color(uint8_t x);

private:
    uint8_t colors_[8];
};

#endif // !BGTILE_HH_
