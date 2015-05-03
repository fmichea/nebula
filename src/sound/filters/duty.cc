#include "duty.hh"

Duty::Duty(const NRX1Proxy& nrx1)
    : nrx1_ (nrx1)
    , frame_ (SAMPLE_RATE)
    , phase_ (8)
    , timer_tick_ (SAMPLE_RATE, 1)
{}

void Duty::reload(int32_t freq) {
    this->timer_tick_ = Converter(SAMPLE_RATE, (2048 - freq) * 4);
    // Reload wave duty for the next sound.
    this->phase_.reset();
    this->pattern_ = this->nrx1_.duty.get();
}

int32_t Duty::filter(int32_t freq) {
    // Depending on the pattern, we return the frequency or not.
    if (this->timer_tick_.next()) {
        unsigned int phase = this->phase_++;
        switch (this->pattern_) {
        case 0: // 00000001 (12.5 %)
            if (phase != 7)
                freq = 0;
            break;
        case 1: // 10000001 (25 %)
            if (0 < phase && phase < 7)
                freq = 0;
            break;
        case 2: // 10000111 (50 %)
            if (0 < phase && phase < 5)
                freq = 0;
            break;
        case 3: // 01111110 (75 %)
            if (phase == 0 || phase == 7)
                freq = 0;
            break;
        };
    }
    // If we got here and the frequency is 0, we pass it as is.
    if (freq == 0)
        return 0;
    // Now we convert the frequency to float and compute a sinusoid.
    unsigned int frame = this->frame_++;
    float _freq = static_cast<float>(freq), tmp;
    tmp = sin(2. * PI * _freq * (frame / static_cast<float>(SAMPLE_RATE)));
    // We have a square wave form, so we either return max or min frequency.
    return tmp < 0 ? MIN_FREQ : MAX_FREQ;
}
