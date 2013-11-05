#include "wave.hh"

WaveForm::WaveForm(const NRX1Proxy& nrx1)
    : frame_ (SAMPLE_RATE), nrx1_ (nrx1), duty_ (2)
{}

void WaveForm::reload() {
    // Reload wave duty for the next sound.
    this->duty_ = this->nrx1_.duty.get();
}

static const float _duty[4] = {0.75, 0.5, 0, -0.5};

int32_t WaveForm::filter(int32_t frequency) {
    int32_t result = 0;
    unsigned int frame = this->frame_++;
    float _freq = static_cast<float>(frequency), tmp;

    tmp = sin(2. * PI * _freq * (frame / static_cast<float>(SAMPLE_RATE)));
    result = (tmp < _duty[this->duty_] ? MIN_FREQ : MAX_FREQ);
    return result;
}
