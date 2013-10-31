#ifndef SDLKBHANDLER_HH_
# define SDLKBHANDLER_HH_
# if _TARGET == 0

#  include <SDL.h>
#  include <pthread.h>

#  include "keyboard/handlers/kbhandler.hh"

class SDLKBHandler : public KBHandler {
public:
    SDLKBHandler(KBEventHandler* eventhandler);
    ~SDLKBHandler();

private:
    pthread_t thread_;
};

# endif // _TARGET == 0
#endif // !SDLKBHANDLER_HH_
