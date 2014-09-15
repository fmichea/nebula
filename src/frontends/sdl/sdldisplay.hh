#ifndef NEBULA_FRONTENDS_SDL_SDLDISPLAY_HH_
# define NEBULA_FRONTENDS_SDL_SDLDISPLAY_HH_

# include <SDL.h>

# include "../abstractdisplay.hh"

# define COEF 3

namespace nebula { namespace frontends { namespace sdl {

class SDLDisplay : public nebula::frontends::AbstractDisplay {
public:
    SDLDisplay(SDL_Window* window);
    ~SDLDisplay();

    void lock();
    void unlock();
    void commit();
    void refresh();

    uint32_t getColor(uint8_t r, uint8_t g, uint8_t b);
    void setPixel(uint8_t x, uint8_t y, uint32_t color);

private:
    SDL_Renderer*   renderer_;
    SDL_Texture*    texture_;

    pthread_mutex_t buffer_mutex_;
    uint32_t*       buffer_;

    pthread_mutex_t commit_mutex_;
    bool            commit_;
};

}}} /* !nebula::frontends::sdl */

#endif /* !NEBULA_FRONTENDS_SDL_SDLDISPLAY_HH_ */
