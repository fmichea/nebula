#include "wave.hh"

WaveForm::WaveForm(const NRX1Proxy& nrx1, const RegisterProxy& nrx3, const NRX4Proxy& nrx4)
    : frame_ (SAMPLE_RATE), nrx1_ (nrx1), nrx3_ (nrx3), nrx4_ (nrx4),
      duty_tick_ (SAMPLE_RATE, 1), duty_phase_ (8)
{}

void WaveForm::reload() {
    // Reload wave duty for the next sound.
    this->duty_phase_.reset();
    this->duty_pattern_ = this->nrx1_.duty.get();
    this->duty_reload_ = true;
}

int32_t WaveForm::filter(int32_t frequency) {
    int32_t result = 0, __freq;
    unsigned int frame = this->frame_++;
    float _freq = static_cast<float>(frequency), tmp;

    if (this->duty_reload_ || this->duty_tick_.next()) {
        __freq = (this->nrx4_.freq_hi.get() << 8) | this->nrx3_.get();
        this->duty_tick_ = Converter(SAMPLE_RATE, 4 * (2048 - __freq));
        switch (this->duty_pattern_) {
        case 0: // 12.5 %
            if (this->duty_phase_.counter() != 7)
                frequency = 0;
            break;
        case 1: // 25 %
            if (this->duty_phase_.counter() != 0 && this->duty_phase_.counter() != 7)
                frequency = 0;
            break;
        case 2: // 50 %
            if (0 < this->duty_phase_.counter() && this->duty_phase_.counter() < 5)
                frequency = 0;
            break;
        case 3: // 75 %
            if (this->duty_phase_.counter() == 0 || this->duty_phase_.counter() == 7)
                frequency = 0;
            break;
        };
        this->duty_phase_++;
        this->duty_reload_ = false;
    }
    if (frequency == 0)
        return 0;
    tmp = sin(2. * PI * _freq * (frame / static_cast<float>(SAMPLE_RATE)));
    result = (tmp < 0 ? MIN_FREQ : MAX_FREQ);
    return result;
}
