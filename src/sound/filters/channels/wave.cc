#include "wave.hh"

namespace nebula { namespace sound { namespace filters { namespace channels {

Wave::Wave(MMU* mmu)
    : Filter()
    , sample_ (32)
    , timer_tick_ (SAMPLE_RATE, 1)
    , mmu_ (mmu)
{}

void Wave::reload(int32_t freq) {
    // Timer generates a clock at this rate:
    this->timer_tick_ = Converter(SAMPLE_RATE, 2 * (2048 - freq));
    // We reset our position in the sample RAM.
    this->sample_.reset();
}

int32_t Wave::filter(int32_t UNUSED(freq)) {
    if (this->timer_tick_.next()) {
        this->sample_++;
    }
    uint8_t byte_idx = this->sample_.counter() / 2;
    uint8_t nibble_idx = this->sample_.counter() % 2;

    uint8_t byte = this->mmu_->read<uint8_t>(0xff30 + byte_idx);
    uint8_t nibble = (byte >> (4 * (1 - nibble_idx))) & 0xf;

    return MIN_FREQ + ((MAX_FREQ - MIN_FREQ) / nibble);
}

}}}} /* !namespaces */
