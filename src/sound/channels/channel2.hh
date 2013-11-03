#ifndef CHANNEL2_HH_
# define CHANNEL2_HH_

# include "channel.hh"

# include "../frequency.hh"
# include "../filters/volume.hh"

class Channel2 : public Channel {
public:
    Channel2(MMU& mmu);

    void fill_stream(int16_t* stream, int len);

private:
    bool update();

private:
    WaveForm wave_;
    VolumeEnvelop volume_;
};

#endif // !CHANNEL2_HH_
