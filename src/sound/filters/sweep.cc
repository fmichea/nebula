#include "sweep.hh"

Sweep::Sweep(int num, NR52Proxy& nr52, const NR10Proxy& nr10)
    : Filter()
    , num_ (num)
    , nr52_ (nr52)
    , nr10_ (nr10)
    , tick_ (SAMPLE_RATE, 128)
    , counter_ (0)
{}

int32_t Sweep::filter(int32_t freq) {
    int32_t diff = 0;

    if (this->enabled_) {
        if (this->tick_.next() && (--this->counter_) == 0) {
            diff = (this->shadow_freq_ >> this->shift_);
            if (this->negate_ == FREQUENCYSWEEP_NEGATE_TRUE) {
                diff *= -1;
            }
            this->diff_ += diff;
        }
        freq += this->diff_;
    }
    if (freq <= 0 || 2048 <= freq) {
        this->nr52_.channel_on[this->num_ - 1].set(0);
    }
    return freq;
}

void Sweep::reload(int32_t freq) {
    this->shadow_freq_ = freq;

    this->tick_.reset();

    this->shift_ = this->nr10_.shift.get();
    this->counter_ = Cycle<unsigned int>(this->nr10_.period.get());

    // The frequency sweep is enabled if either the sweep period or shift are
    // non-zero, cleared otherwise.
    this->enabled_ = (this->counter_.maximum() != 0 || this->shift_);

    // The negate flag decides if this sweep should increase or decrease the
    // frequency of the sound wave.
    switch (this->nr10_.negate.get()) {
    case 0x1:
        this->negate_ = FREQUENCYSWEEP_NEGATE_TRUE;
        break;
    case 0x0:
        this->negate_ = FREQUENCYSWEEP_NEGATE_FALSE;
        break;
    };

    this->diff_ = 0;
}
