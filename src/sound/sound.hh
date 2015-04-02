#ifndef NEBULA_SOUND_SOUND_HH_
# define NEBULA_SOUND_SOUND_HH_

# include <SDL.h>

# include "../logging.hh"
# include "../memory/mmu.hh"

# include "filters/channels/square.hh"
# include "filters/filter.hh"
# include "filters/length.hh"
# include "filters/sweep.hh"
# include "filters/volume.hh"
# include "filters/wave.hh"

# include "channel.hh"
# include "constants.hh"

class Sound {
public:
    Sound(MMU* mmu);
    ~Sound();

    void fill_stream(Uint8* stream, int len);

private:
    MMU* mmu_;
    Channel* channels_[NB_GB_CHANNELS];

    // FIXME: move this soon.
    SDL_AudioSpec   spec_;
};

#endif // !NEBULA_SOUND_SOUND_HH_
