#ifndef GPU_HH_
# define GPU_HH_

# include <list>
# include <string.h>

# include "../memory/mmu.hh"
# include "../z80/interrupts.hh"
# include "bgwtile.hh"
# include "sprites.hh"
# include "timer.hh"

# include "../frontends/abstractdisplay.hh"

# define LCDC_MODE_0_CLKS   201
# define LCDC_MODE_1_CLKS   4560
# define LCDC_MODE_2_CLKS   77
# define LCDC_MODE_3_CLKS   169

# define LCDC_MODE_0 0
# define LCDC_MODE_1 1
# define LCDC_MODE_2 2
# define LCDC_MODE_3 3

class GPU
{
public:
    GPU(nebula::frontends::AbstractDisplay* display, MMU* mmu);
    ~GPU();

    void do_cycle(uint8_t cycles);

private:
    void draw_line();
    uint32_t bgp_to_color(uint8_t idx, uint8_t pal);
    uint32_t obp_to_color(uint8_t idx, uint8_t pal);

    MMU* mmu_;
    Timer timer_;
    int16_t wait_count;

    nebula::frontends::AbstractDisplay* display_;
    uint32_t bgcolors_[4];
};

#endif // !GPU_HH_
