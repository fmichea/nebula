#ifndef CHANNEL__HH_
# define CHANNEL__HH_

#include "sound/channels/channel.hh"

// Channel 1: Tone & Sweep
class Channel1 : public Channel {
public:
    Channel1(const MMU& mmu);

    void fill_stream(int16_t* stream, int len);
};

#endif // !CHANNEL__HH_
