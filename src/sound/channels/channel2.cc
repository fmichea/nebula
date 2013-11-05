#include "sound/channels/channel2.hh"

Channel2::Channel2(MMU& mmu)
    : Channel(mmu), wave_(WAVEFORM_SQUARE), volume_ (mmu.NR22)
{}

void Channel2::fill_stream(int16_t* stream, int len) {
    float data = 0;

    if (!this->update())
        return;
    for (int it = 0; it < len; ++it) {
        data = this->wave_.next();
        data = this->volume_.filter(data);
        stream[it] = data;
    }
}

bool Channel2::update() {
    if (this->mmu_.NR24.restart.get()) {
        uint16_t _freq = ((this->mmu_.NR24.freq_hi.get()) << 8) | this->mmu_.NR23.get();
        float freq = static_cast<float>(_freq);

        this->wave_.set_frequency(131072. / (2048. - freq));
        this->wave_.set_pattern(this->mmu_.NR21.pattern_duty.get());

        this->volume_.reset();
    }
    return true;
}
