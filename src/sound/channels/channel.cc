#include "channel.hh"

Channel::Channel(int num, NR52Proxy& nr52, const RegisterProxy& nrx3, const NRX4Proxy& nrx4, const std::list<Filter*>& filters)
    : num_ (num), frequency_ (261), filters_ (filters), nr52_ (nr52), nrx3_ (nrx3), nrx4_ (nrx4)
{}

Channel::~Channel()
{
    std::list<Filter*>::const_iterator it;

    for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
        delete (*it);
    }
}

void Channel::fill_stream(int16_t* stream, unsigned int len) {
    std::list<Filter*>::const_iterator it;

    this->update();
    for (unsigned int x = 0; x < len; ++x) {
        int32_t data = this->frequency_;

        for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
            data = (*it)->filter(data);
        }
        stream[x] = static_cast<int16_t>(data);
    }
}

void Channel::update() {
    uint16_t freq;
    std::list<Filter*>::const_iterator it;

    if (this->nrx4_.restart.get()) { // Reload sound channel.
        // Reload sound frequency.
        freq = (this->nrx4_.freq_hi.get() << 8) | this->nrx3_.get();
        this->frequency_ = 131072 / (2048 - freq);
        logging::info("frequency: %d", this->frequency_);

        for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
            (*it)->reload();
        }
        this->nr52_.channel_on[this->num_ - 1].set(1);
    }
}
