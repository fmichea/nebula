#include "sdlkbhandler.hh"

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

static void* handle_events(void* data);

SDLKBHandler::SDLKBHandler(KBEventHandler* eventhandler)
    : KBHandler(eventhandler)
{
    pthread_attr_t attr;

    // Initialize SDL sub systems.
    SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_EVENTS);

    // Create the event manager thread.
    pthread_attr_init(&attr);
    pthread_create(&this->thread_, &attr, handle_events, eventhandler);
    pthread_attr_destroy(&attr);
}

SDLKBHandler::~SDLKBHandler() {
    // Kill the thread.
    pthread_cancel(this->thread_);
    pthread_join(this->thread_, NULL);

    // Quit submodule.
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_EVENTS);
}

static void* handle_events(void* data) {
    SDL_Event       event;
    s_kbh_event     ret;
    KBEventHandler* eventhandler = static_cast<KBEventHandler*>(data);

    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            for (int it = 0; it < KEY_ASSOC_SZ; ++it) {
                if (event.key.keysym.sym == key_assoc[it].sdlkey) {
                    if (event.type == SDL_KEYDOWN)
                        ret.type = KBH_EVENT_TYPE_KEYDOWN;
                    else
                        ret.type = KBH_EVENT_TYPE_KEYUP;
                    ret.key = key_assoc[it].kbhkey;
                    eventhandler->handle_event(&ret);
                    return 0;
                }
            }
        } else if (event.type == SDL_QUIT) {
            ret.key = KBH_KEY_QUIT;
            ret.type = KBH_EVENT_TYPE_KEYDOWN;
            eventhandler->handle_event(&ret);
            break;
        }
    }
    return NULL;
}
