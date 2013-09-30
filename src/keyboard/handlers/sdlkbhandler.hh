#ifndef SDLKBHANDLER_HH_
# define SDLKBHANDLER_HH_
# if _TARGET == 0

#  include <SDL/SDL.h>

#  include "kbhandler.hh"

class SDLKBHandler : public KBHandler {
public:
    SDLKBHandler(KBEventHandler* eventhandler);
};

# endif // _TARGET == 0
#endif // !SDLKBHANDLER_HH_
