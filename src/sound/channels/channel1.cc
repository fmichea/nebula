#include "sound/channels/channel1.hh"

Channel1::Channel1(MMU& mmu)
    : Channel(mmu),
      wave_ (WAVEFORM_SQUARE),
      volume_ (mmu.NR12),
      sweep_ (SAMPLE_RATE, 128),
      length_tick_ (SAMPLE_RATE, 256),
      length_ (0),
      length_enabled_ (false)
{
    this->wave_.set_frequency(440);
}

void Channel1::fill_stream(int16_t* stream, int len) {
    int16_t data;

    if (!this->update())
        return;
    for (int it = 0; it < len; ++it) {
#if 0
        if (this->sweep_time_ && this->sweep_.next()) {
            logging::info("sweep shift!");
            float freq = this->wave_.frequency();
            logging::info("freq = %f", freq);
            float freq2 = freq / (1 << this->sweep_shift_);
            if (this->sweep_way_) freq -= freq2;
            else freq += freq2;
            logging::info("freq = %f", freq);
            this->wave_.set_frequency(freq);
            this->sweep_time_ -= 1;
        }
#endif

        data = this->wave_.next();
        //data = this->volume_.filter(data);
        stream[it] = data;
    }
}

bool Channel1::update() {
    uint16_t freq = (this->mmu_.NR14.freq_hi.get()) << 8 | this->mmu_.NR13.get();

    this->freq_ = 131072 / (2048 - freq);
#if 0
    if (this->mmu_.NR14.restart.get()) { // Restart sound.
        logging::info("lol = %f (%d)", freq, _freq);
        logging::info("sweep time = %d", this->mmu_.NR10.sweep_time.get());
        this->mmu_.NR14.restart.set(0);

        // Update the current frequency of the sound.
        this->wave_.set_frequency(440);
        this->wave_.set_pattern(this->mmu_.NR11.pattern_duty.get());

        // Length.
        this->length_enabled_ = false;
        if (this->mmu_.NR14.repeat.get()) {
            this->length_enabled_ = true;
            this->length_ = 64 - this->mmu_.NR11.sound_length.get();
            logging::info("length = %d", this->length_);
        }

        // Configure sweep.
        this->sweep_time_ = this->mmu_.NR10.sweep_time.get();
        if (this->sweep_time_ != 0) {
            this->sweep_shift_ = this->mmu_.NR10.sweep_shift.get();
            this->sweep_way_ = this->mmu_.NR10.sweep_way.get();
            this->sweep_.reset();
        }

        // Volume.
        this->volume_.reset();
        logging::info("volume: %d", this->mmu_.NR12.volume.get());
        logging::info("way: %d", this->mmu_.NR12.envelop_way.get());
        logging::info("sweep: %d", this->mmu_.NR12.envelop_sweep.get());

    } else if (this->length_enabled_ && this->length_tick_.next()){
        this->length_ -= 1;
    }
#endif
    //return (!this->length_enabled_ || this->length_ != 0);
    return true;
}
