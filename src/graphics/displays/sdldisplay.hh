#ifndef SDLDISPLAY_HH_
# define SDLDISPLAY_HH_

# if _TARGET == 0

#  include <SDL.h>

#  include "display.hh"

#  define COEF 2

class SDLDisplay : public Display
{
public:
    SDLDisplay(const char* windowName);
    virtual ~SDLDisplay();

    void lock();
    void unlock();
    void commit();

    int setPixel(uint8_t x, uint8_t y, uint32_t color);

    uint32_t getColor(uint8_t r, uint8_t g, uint8_t b);

private:
    SDL_Window*     window_;
    SDL_Renderer*   renderer_;
    SDL_Texture*    texture_;

    uint32_t*       buffer_;
};

# endif // _DISPLAY_BACKEND == 0
#endif // !SDLDISPLAY_HH_
