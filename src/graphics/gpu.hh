#ifndef GPU_HH_
# define GPU_HH_

# include <list>
# include <string.h>

# include "../memory/mmu.hh"
# include "bgwtile.hh"
# include "sprites.hh"
# include "timer.hh"

# include "displays/display.hh"
# include "displays/sdldisplay.hh"

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
