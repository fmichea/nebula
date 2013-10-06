#ifndef GPU_HH_
# define GPU_HH_

# include <list>
# include <string.h>

# include "graphics/bgwtile.hh"
# include "graphics/sprites.hh"
# include "graphics/timer.hh"
# include "memory/mmu.hh"
# include "z80/interrupts.hh"

# include "graphics/displays/display.hh"
# include "graphics/displays/sdldisplay.hh"

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
    GPU(MMU& mmu);
    ~GPU();

    void do_cycle(uint8_t cycles);

private:
    void draw_line();

    MMU&            mmu_;
    Timer           timer_;
    int16_t         wait_count;

    Display*        display_;
    uint32_t        colors_[4];
};

#endif // !GPU_HH_
