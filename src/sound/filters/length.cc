#include "length.hh"

static void _reload_length_counter(void* data, uint16_t addr);

Length::Length(int num, MMU* mmu, NR52Proxy& nr52, const NRX1Proxy& nrx1, const NRX4Proxy& nrx4)
    : Filter()
    , tick_ (SAMPLE_RATE, 128)
    , num_ (num)
    , nr52_ (nr52)
    , nrx1_ (nrx1)
    , nrx4_ (nrx4)
{
    // In addition to the standard trigger event, the length counter can be
    // reloaded any time by writing to NRx1.
    mmu->subscribe(nrx1.addr(), WatchType::RO, _reload_length_counter, this);
}

void Length::reload(int32_t UNUSED(freq)) {
    this->enabled_ = (this->nrx4_.repeat.get() == 1);
    this->reload_counter();
    // Reset the tick.
    this->tick_.reset();
}

void Length::reload_counter() {
    // Load the length, channel 3 as a 256 maximum, others use 64.
    this->length_ = this->num_ == 3 ? 256 : 64;
    this->length_ = this->nrx1_.sound_length.get();
}

int32_t Length::filter(int32_t freq) {
    if (this->enabled_) {
        if (this->length_ != 0 && this->tick_.next()) {
            this->length_ -= 1;
            //logging::info("Length tick: %d", this->length_);
            this->nr52_.channel_on[this->num_ - 1].set((this->length_ ? 1 : 0));
        }
        if (this->length_ == 0)
            return 0;
    }
    return freq;
}

static void _reload_length_counter(void* data, uint16_t UNUSED(addr)) {
    Length* length = static_cast<Length*>(data);
    length->reload_counter();
}
