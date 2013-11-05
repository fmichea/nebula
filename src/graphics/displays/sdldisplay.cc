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

    this->texture_ = SDL_CreateTexture(this->renderer_,
                                       SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       WIDTH * COEF, HEIGHT * COEF);

    this->buffer_ = new uint32_t[COEF * COEF * WIDTH * HEIGHT];

    // Clear the surface.
    SDL_SetRenderDrawColor(this->renderer_, 255, 255, 255, 100);
    SDL_RenderClear(this->renderer_);
}

SDLDisplay::~SDLDisplay()
{
    SDL_DestroyRenderer(this->renderer_);
    SDL_DestroyWindow(this->window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    delete[] this->buffer_;
}

void SDLDisplay::lock(void) {
    //SDL_LockTexture(this->texture_, NULL, NULL, NULL);
}

void SDLDisplay::unlock(void) {
    //SDL_UnlockTexture(this->texture_);
}

void SDLDisplay::commit(void) {
    this->lock();
    SDL_RenderClear(this->renderer_);
    SDL_UpdateTexture(this->texture_, NULL, this->buffer_,
                      sizeof (uint32_t) * COEF * WIDTH);
    SDL_RenderCopy(this->renderer_, this->texture_, NULL, NULL);
    SDL_RenderPresent(this->renderer_);
    this->unlock();
}

int SDLDisplay::setPixel(uint8_t x, uint8_t y, uint32_t c) {
    int y_ = y * COEF * COEF * WIDTH;
    for (int it = 0; it < COEF; ++it) {
        for (int it2 = 0; it2 < COEF; ++it2)
        this->buffer_[y_ + it * COEF * WIDTH + x * COEF + it2] = c;
    }
    return 1;
}

#define _RGB(R, G, B) ((R << 16) | (G << 8) | B)

uint32_t SDLDisplay::getColor(uint8_t r, uint8_t g, uint8_t b)
{
    return _RGB(r, g, b);
}
