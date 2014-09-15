#include "sdldisplay.hh"

namespace nebula { namespace frontends { namespace sdl {

SDLDisplay::SDLDisplay(SDL_Window* window) {
    Uint32 flags;

    flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    this->renderer_ = SDL_CreateRenderer(window, -1, flags);

    this->texture_ = SDL_CreateTexture(this->renderer_,
                                       SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       WIDTH * COEF, HEIGHT * COEF);

    // Clear the surface.
    SDL_SetRenderDrawColor(this->renderer_, 255, 255, 255, 100);
    SDL_RenderClear(this->renderer_);

    // Initialize the mutex and buffer.
    pthread_mutex_init(&this->buffer_mutex_, NULL);
    this->buffer_ = new uint32_t[COEF * COEF * WIDTH * HEIGHT];

    // Finally initialize commit and mutex.
    pthread_mutex_init(&this->commit_mutex_, NULL);
    this->commit_ = false;
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyTexture(this->texture_);
    SDL_DestroyRenderer(this->renderer_);
    delete[] this->buffer_;
    pthread_mutex_destroy(&this->buffer_mutex_);
}

void SDLDisplay::lock() {
    pthread_mutex_lock(&this->buffer_mutex_);
    pthread_mutex_lock(&this->commit_mutex_);
}

void SDLDisplay::unlock() {
    pthread_mutex_unlock(&this->commit_mutex_);
    pthread_mutex_unlock(&this->buffer_mutex_);
}

void SDLDisplay::commit() {
    this->lock();
    this->commit_ = true;
    this->unlock();
}

void SDLDisplay::refresh() {
    this->lock();
    if (this->commit_) {
        SDL_RenderClear(this->renderer_);
        SDL_UpdateTexture(this->texture_, NULL, this->buffer_,
                          sizeof (uint32_t) * COEF * WIDTH);
        SDL_RenderCopy(this->renderer_, this->texture_, NULL, NULL);
        SDL_RenderPresent(this->renderer_);
        this->commit_ = false;
    }
    this->unlock();
}

#define _RGB(R, G, B) ((R << 16) | (G << 8) | B)

uint32_t SDLDisplay::getColor(uint8_t r, uint8_t g, uint8_t b) {
    return _RGB(r, g, b);
}

void SDLDisplay::setPixel(uint8_t x, uint8_t y, uint32_t c) {
    int y_ = y * COEF * COEF * WIDTH;

    for (int it = 0; it < COEF; ++it) {
        for (int it2 = 0; it2 < COEF; ++it2)
            this->buffer_[y_ + it * COEF * WIDTH + x * COEF + it2] = c;
    }
}

}}} /* !nebula::frontends::sdl */
