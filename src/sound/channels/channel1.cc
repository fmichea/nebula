#include "sound/channels/channel1.hh"

Channel1::Channel1(MMU& mmu)
    : Channel(mmu),
      wave_ (WAVEFORM_SQUARE),
      sweep_ (SAMPLE_RATE, 128),
      length_ (SAMPLE_RATE, 256)
{}

void Channel1::fill_stream(int16_t* stream, int len) {
    for (int it = 0; it < len; ++it) {
    }
}

void Channel1::update() {
    uint16_t freq = (this->mmu_.NR14.get() & 7) << 8 | this->mmu_.NR13.get();

    if (this->mmu_.NR14.get() >> 7) { // Restart sound.
        this->mmu_.NR14.set(this->mmu_.NR14.get() & 0x7f);

        // Update the current frequency of the sound.
        this->wave_.set_frequency(131072. / (2048. - freq));
    }
}
