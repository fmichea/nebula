#ifndef SOUND_HH_
# define SOUND_HH_

# include <cmath>

# include <SDL/SDL.h>

# include "logging.hh"
# include "memory/mmu.hh"

# define PI 3.14159265

class Sound {
public:
    Sound(const MMU& mmu);
    ~Sound();
};

#endif // !SOUND_HH_
