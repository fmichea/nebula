#include "sdlkbhandler.hh"

#define KEY_ASSOC_SZ 9
struct {
    SDLKey sdlkey;
    e_kbh_key kbhkey;
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

static int handle_event(const SDL_Event* e);

static KBEventHandler* _eventhandler = nullptr;

SDLKBHandler::SDLKBHandler(KBEventHandler* eventhandler)
    : KBHandler(eventhandler)
{
    _eventhandler = this->eventhandler_;
    SDL_SetEventFilter(handle_event);
}

static int handle_event(const SDL_Event* e) {
    s_kbh_event event;

    if (_eventhandler == nullptr)
        return 1;
    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        for (int it = 0; it < KEY_ASSOC_SZ; ++it) {
            if (e->key.keysym.sym == key_assoc[it].sdlkey) {
                if (e->type == SDL_KEYDOWN)
                    event.type = KBH_EVENT_TYPE_KEYDOWN;
                else
                    event.type = KBH_EVENT_TYPE_KEYUP;
                event.key = key_assoc[it].kbhkey;
                _eventhandler->handle_event(&event);
                return 0;
            }
        }
    }
    return 1;
}
