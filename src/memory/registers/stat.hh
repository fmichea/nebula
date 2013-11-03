#ifndef STAT_HH_
# define STAT_HH_

# include "bitproxy.hh"
# include "register.hh"

class STATProxy : public RegisterProxy {
public:
    STATProxy() : RegisterProxy() {}

    STATProxy(uint8_t* reg, uint16_t addr)
        : RegisterProxy(reg, addr)
    {
        this->coin_int = BitProxy(reg, 6, 0x1);
        this->OAM_int = BitProxy(reg, 5, 0x1);
        this->VBlank_int = BitProxy(reg, 4, 0x1);
        this->HBlank_int = BitProxy(reg, 3, 0x1);
        this->coin_flag = BitProxy(reg, 2, 0x1);
        this->mode = BitProxy(reg, 0, 0x3);
    }

    BitProxy coin_int; // LYC = LY Coincidence Interrupt (1 = Enable)
    BitProxy OAM_int; // Mode 2 (OAM Interrupt)
    BitProxy VBlank_int; // Mode 1 (V-Blank Interrupt)
    BitProxy HBlank_int; // Mode 0 (H-Blank Interrupt)
    BitProxy coin_flag; // Coincidence Flag
    BitProxy mode; // Mode Flag (H-Blank, V-Blank, OAM, ...)
};

#endif // !STAT_HH_
