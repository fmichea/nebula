#include "length.hh"

Length::Length(int num, NR52Proxy& nr52, const NRX1Proxy& nrx1, const NRX4Proxy& nrx4)
    : Filter(), tick_ (SAMPLE_RATE, 128), num_ (num), nr52_ (nr52), nrx1_ (nrx1), nrx4_ (nrx4)
{}

void Length::reload() {
    this->enabled_ = (this->nrx4_.repeat.get() == 1);
    this->length_ = 64 - this->nrx1_.sound_length.get();
    this->tick_.reset();
}

int32_t Length::filter(int32_t frequency) {
    if (this->enabled_) {
        if (this->length_ != 0 && this->tick_.next()) {
            this->length_ -= 1;
            //logging::info("Length tick: %d", this->length_);
            this->nr52_.channel_on[this->num_ - 1].set((this->length_ ? 1 : 0));
        }
        if (this->length_ == 0)
            return 0;
    }
    return frequency;
}
