#ifndef CHANNEL__HH_
# define CHANNEL__HH_

# include "../frequency.hh"
# include "channel.hh"

// Channel 1: Tone & Sweep
class Channel1 : public Channel {
public:
    Channel1(MMU& mmu);

    void fill_stream(int16_t* stream, int len);

private:
    void update();

private:
    WaveForm wave_;
    Converter sweep_;
    Converter length_;
};

#endif // !CHANNEL__HH_
