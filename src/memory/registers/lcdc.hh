#ifndef LCDC_HH_
# define LCDC_HH_

# include <stdint.h>

# include "memory/registers/register.hh"
# include "memory/registers/bitproxy.hh"

class LCDCProxy : public RegisterProxy {
public:
    LCDCProxy() : RegisterProxy() {}

    LCDCProxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->LDE = BitProxy(reg, 7, 0x1);
        this->WTMDS = BitProxy(reg, 6, 0x1);
        this->WDE = BitProxy(reg, 5, 0x1);
        this->BGWTDS = BitProxy(reg, 4, 0x1);
        this->BGTMDS = BitProxy(reg, 3, 0x1);
        this->OBJSS = BitProxy(reg, 2, 0x1);
        this->OBJSDE = BitProxy(reg, 1, 0x1);
        this->BGD = BitProxy(reg, 0, 0x1);
    }

    BitProxy LDE; // LCD Display Enable
    BitProxy WTMDS; // Window Tile Map Display Select
    BitProxy WDE; // Window Display Enable
    BitProxy BGWTDS; // BG & Window Tile Data Select
    BitProxy BGTMDS; // BG Tile Map Data Select
    BitProxy OBJSS; // OBJ (Sprite) Size
    BitProxy OBJSDE; // OBJ (Sprite) Display Enable
    BitProxy BGD; // BG Display
};

#endif // !LCDC_HH_
