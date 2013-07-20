#include "sdldisplay.hh"

#if _DISPLAY_BACKEND == 0

SDLDisplay::SDLDisplay(const char* windowName)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK);
    this->screen_ = SDL_SetVideoMode(WIDTH * COEF, HEIGHT * COEF, 32,
                                     SDL_SWSURFACE);
    SDL_WM_SetCaption(windowName, 0);

    SDL_FillRect(this->screen_, 0, SDL_MapRGB(this->screen_->format, 255, 255, 255));

    this->rect_.w = COEF;
    this->rect_.h = COEF;
}

SDLDisplay::~SDLDisplay()
{
    SDL_Quit();
}

void SDLDisplay::lock(void) {
    SDL_LockSurface(this->screen_);
}

void SDLDisplay::unlock(void) {
    SDL_UnlockSurface(this->screen_);
}

void SDLDisplay::commit(void) {
    SDL_Flip(this->screen_);
}

int SDLDisplay::setPixel(uint8_t x, uint8_t y, uint32_t color) {
    this->rect_.x = COEF * x; this->rect_.y = COEF * y;
    return SDL_FillRect(this->screen_, &(this->rect_), color);
}

uint32_t SDLDisplay::getColor(uint8_t r, uint8_t g, uint8_t b)
{
    return SDL_MapRGB(this->screen_->format, r, g, b);
}

#endif // _DISPLAY_BACKEND == 0
