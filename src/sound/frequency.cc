#include "sound/frequency.hh"

WaveForm::WaveForm(waveform_type type)
    : type_ (type), counter_ (0), cur_freq_ (0)
{}

void WaveForm::set_frequency(unsigned int freq) {
    this->cur_freq_ = freq;
}

int16_t WaveForm::next() {
    float freq = static_cast<float>(this->cur_freq_), res;

    res = static_cast<float>(this->counter_) / SAMPLE_RATE;
    res = sin(2. * PI * res * freq);
    switch (this->type_) {
    case WAVEFORM_SINE:
        break;
    case WAVEFORM_SQUARE:
        res = (res < 0 ? MIN_FREQ : MAX_FREQ);
        break;
    };
    return res;
}

unsigned int WaveForm::frequency() const {
    return this->cur_freq_;
}


Converter::Converter(unsigned int base_rate, unsigned int final_rate)
    : counter_ (0)
{
    if (base_rate < final_rate) {
        logging::error("Should not create a divider with base rate greater "
                       "than final rate");
    }
    this->divider_ = base_rate / final_rate;
}

bool Converter::next() {
    this->counter_ = (this->counter_ + 1) % this->divider_;
    return this->counter_ == 0;
}

void Converter::reset() {
    this->counter_ = 0;
}
