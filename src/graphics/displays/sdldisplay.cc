#include "sdldisplay.hh"

SDLDisplay::SDLDisplay(const char* windowName)
{
    Uint32 flags;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    this->window_ = SDL_CreateWindow(windowName,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     WIDTH * COEF,
                                     HEIGHT * COEF,
                                     SDL_WINDOW_SHOWN);

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    this->renderer_ = SDL_CreateRenderer(this->window_, -1, flags);

    // Clear the surface.
    SDL_SetRenderDrawColor(this->renderer_, 255, 255, 255, 100);
    SDL_RenderClear(this->renderer_);

    this->rect_.w = COEF;
    this->rect_.h = COEF;
}

SDLDisplay::~SDLDisplay()
{
    SDL_DestroyRenderer(this->renderer_);
    SDL_DestroyWindow(this->window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void SDLDisplay::lock(void) {
    //SDL_LockSurface(this->screen_);
}

void SDLDisplay::unlock(void) {
    //SDL_UnlockSurface(this->screen_);
}

void SDLDisplay::commit(void) {
    SDL_RenderPresent(this->renderer_);
}

#define _RGB(R, G, B) ((R << 16) | (G << 8) | B)
#define _R(Color) ((Color >> 16) & 0xff)
#define _G(Color) ((Color >> 8) & 0xff)
#define _B(Color) (Color & 0xff)

int SDLDisplay::setPixel(uint8_t x, uint8_t y, uint32_t c) {
    this->rect_.x = COEF * x; this->rect_.y = COEF * y;
    SDL_SetRenderDrawColor(this->renderer_, _R(c), _G(c), _B(c), 100);
    return SDL_RenderFillRect(this->renderer_, &this->rect_);
}

uint32_t SDLDisplay::getColor(uint8_t r, uint8_t g, uint8_t b)
{
    return _RGB(r, g, b);
}
