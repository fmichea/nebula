#include "sound/channels/channel1.hh"

Channel1::Channel1(const MMU& mmu)
    : Channel(mmu)
{}

void Channel1::fill_stream(int16_t* stream, int len) {
    uint16_t freq = (this->mmu_.NR14.get() & 7) << 8 | this->mmu_.NR13.get();

    freq = 131072. / (2048. - freq);
    this->freq_to_stream(stream, len, freq);
}
