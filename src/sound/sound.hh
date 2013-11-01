#ifndef SOUND_HH_
# define SOUND_HH_

# include <cmath>

# include <SDL/SDL.h>

# include "logging.hh"
# include "memory/mmu.hh"

# include "sound/constants.hh"

class Sound {
public:
    Sound(const MMU& mmu);
    ~Sound();
};

#endif // !SOUND_HH_
