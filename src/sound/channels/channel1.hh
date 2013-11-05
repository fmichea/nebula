#ifndef CHANNEL1_HH_
# define CHANNEL1_HH_

# include "../filters/volume.hh"
# include "../frequency.hh"
# include "channel.hh"

// Channel 1: Tone & Sweep
class Channel1 : public Channel {
public:
    Channel1(MMU& mmu);

    void fill_stream(int16_t* stream, int len);

private:
    bool update();

private:
    WaveForm wave_;
    VolumeEnvelop volume_;

    Converter sweep_;
    unsigned int sweep_time_;
    unsigned int sweep_way_;
    unsigned int sweep_shift_;

    Converter length_tick_;
    unsigned int length_;
    bool length_enabled_;
};

#endif // !CHANNEL1_HH_
