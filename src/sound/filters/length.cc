#include "length.hh"

Length::Length(const NRX1Proxy& nrx1, const NRX4Proxy& nrx4)
    : tick_ (SAMPLE_RATE, 128), enabled_ (false), nrx1_ (nrx1), nrx4_ (nrx4)
{}

void Length::reload() {
    this->enabled_ = (this->nrx4_.repeat.get() == 1);
    this->length_ = 64 - this->nrx1_.sound_length.get();
    this->tick_.reset();
}

int32_t Length::filter(int32_t frequency) {
    if (this->enabled_) {
        if (this->length_ != 0 && this->tick_.next()) {
            logging::info("Length tick.");
            this->length_ -= 1;
        }
        if (this->length_ == 0)
            return 0;
    }
    return frequency;
}
