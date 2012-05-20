#ifndef GPU_HH_
# define GPU_HH_

# include <SDL/SDL.h>

# include "timer.hh"
# include "bgwtile.hh"
# include "../memory/mmu.hh"

# define COEF 2

# define WIDTH 160
# define HEIGHT 144

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
    SDL_Surface*    screen_;
    unsigned int    colors_[4];
    Timer           timer_;
    int16_t         wait_count;
};

#endif // !GPU_HH_
