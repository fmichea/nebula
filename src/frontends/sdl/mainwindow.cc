#include "mainwindow.hh"

#define KEY_ASSOC_SZ 9
struct {
    SDL_Keycode sdlkey;
    e_kbh_key   kbhkey;
} key_assoc[KEY_ASSOC_SZ] = {
    { .sdlkey = SDLK_ESCAPE, .kbhkey = KBH_KEY_QUIT },

    { .sdlkey = SDLK_UP, .kbhkey = KBH_KEY_UP },
    { .sdlkey = SDLK_RIGHT, .kbhkey = KBH_KEY_RIGHT },
    { .sdlkey = SDLK_DOWN, .kbhkey = KBH_KEY_DOWN },
    { .sdlkey = SDLK_LEFT, .kbhkey = KBH_KEY_LEFT },

    { .sdlkey = SDLK_a, .kbhkey = KBH_KEY_A },
    { .sdlkey = SDLK_q, .kbhkey = KBH_KEY_B },

    { .sdlkey = SDLK_SPACE, .kbhkey = KBH_KEY_START },
    { .sdlkey = SDLK_RETURN, .kbhkey = KBH_KEY_SELECT },
};

namespace nebula { namespace frontends { namespace sdl {

MainWindow::MainWindow(const char* windowName)
    : AbstractMainWindow()
#ifdef _SOUND
    , sound_ (nullptr)
#endif
{
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS);

    this->window_ = SDL_CreateWindow(windowName,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     WIDTH * COEF,
                                     HEIGHT * COEF,
                                     SDL_WINDOW_SHOWN);

    this->display_ = new SDLDisplay(this->window_);
}

MainWindow::~MainWindow() {
    // Destroy the display.
    delete this->display_;
    this->display_ = nullptr;
    // Destroy the window.
    SDL_DestroyWindow(this->window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS);
}

void MainWindow::run() {
    SDL_Event       event;
    s_kbh_event     ret;

#ifdef _SOUND
    this->sound_ = new Sound(this->mmu_);
#endif

    while (!this->mmu_->stopped) {
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                for (int it = 0; it < KEY_ASSOC_SZ; ++it) {
                    if (event.key.keysym.sym == key_assoc[it].sdlkey) {
                        if (event.type == SDL_KEYDOWN)
                            ret.type = KBH_EVENT_TYPE_KEYDOWN;
                        else
                            ret.type = KBH_EVENT_TYPE_KEYUP;
                        ret.key = key_assoc[it].kbhkey;
                        this->kb_->handle_event(&ret);
                    }
                }
            } else if (event.type == SDL_QUIT) {
                ret.key = KBH_KEY_QUIT;
                ret.type = KBH_EVENT_TYPE_KEYDOWN;
                this->kb_->handle_event(&ret);
                break;
            }
        }
        this->display_->refresh();
    }

#ifdef _SOUND
    delete this->sound_;
#endif
}

}}} /* !nebula::frontends::sdl */
