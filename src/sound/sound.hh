#ifndef SOUND_HH_
# define SOUND_HH_

# include <SDL.h>

# include "logging.hh"
# include "memory/mmu.hh"

# include "sound/channels/channel1.hh"
# include "sound/constants.hh"

class Sound {
public:
    Sound(const MMU& mmu);
    ~Sound();

    void fill_stream(Uint8* stream, int len);

private:
    const MMU&      mmu_;
    Channel*        channels_[NB_GB_CHANNELS];

    // FIXME: move this soon.
    SDL_AudioSpec   spec_;
};

#endif // !SOUND_HH_
