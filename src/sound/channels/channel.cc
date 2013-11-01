#include "sound/channels/channel.hh"

Channel::Channel(const MMU& mmu)
    : mmu_ (mmu), count_stream_ (0)
{}

Channel::~Channel()
{}

void Channel::freq_to_stream(int16_t* stream, int len, int freq_) {
    float freq = static_cast<float>(freq_), cur = 0;

    for (int it = 0; it < len; ++it) {
        cur = static_cast<float>(this->count_stream_) / SAMPLE_RATE;
        cur = sin(2. * PI * freq * cur);
        stream[it] = (cur < 0 ? MIN_FREQ : MAX_FREQ);
        this->count_stream_ = (this->count_stream_ + 1) % SAMPLE_RATE;
    }
}
