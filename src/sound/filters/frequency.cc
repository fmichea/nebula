#include "frequency.hh"

QuandrangularChannel::QuandrangularChannel()
    : Filter()
{}

void QuandrangularChannel::reload() {}

int32_t QuandrangularChannel::filter(int32_t freq) {
    if (0 <= freq && freq < 2048)
        freq = 131072 / (2048 - freq) / 2;
    else
        freq = 0;
    //logging::info("frequency: %d", this->frequency_);
    return freq;
}
