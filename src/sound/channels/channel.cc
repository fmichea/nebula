#include "channel.hh"

static void _reload_channel_callback(void* data, uint16_t addr);

Channel::Channel(MMU* mmu, int num, const RegisterProxy& nrx3, const NRX4Proxy& nrx4, const std::list<Filter*>& filters)
    : num_ (num), frequency_ (261), filters_ (filters), nr52_ (mmu->NR52), nrx3_ (nrx3), nrx4_ (nrx4)
{
    mmu->subscribe(nrx4.addr(), WatchType::WO, _reload_channel_callback, this);
    for (Filter* filter : this->filters_)
        filter->reload();
}

Channel::~Channel() {
    std::list<Filter*>::const_iterator it;

    for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
        delete (*it);
    }
}

void Channel::fill_stream(int16_t* stream, unsigned int len) {
    std::list<Filter*>::const_iterator it;

    for (unsigned int x = 0; x < len; ++x) {
        if (!this->nr52_.channel_on[this->num_ - 1].get())
            break;

        int32_t data = this->frequency_;

        for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
            data = (*it)->filter(data);
        }
        stream[x] = static_cast<int16_t>(data);
    }
}

void Channel::reload() {
    uint16_t freq;
    std::list<Filter*>::const_iterator it;

    if (this->nrx4_.restart.get()) { // Reload sound channel.
        // Reload sound frequency.
        freq = (this->nrx4_.freq_hi.get() << 8) | this->nrx3_.get();
        this->frequency_ = freq;

        for (it = this->filters_.begin(); it != this->filters_.end(); it++) {
            (*it)->reload();
        }
        this->nr52_.channel_on[this->num_ - 1].set(1);
    }
}

static void _reload_channel_callback(void* data, uint16_t UNUSED(addr)) {
    Channel* channel = static_cast<Channel*>(data);
    channel->reload();
}
