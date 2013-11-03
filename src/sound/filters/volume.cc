#include "volume.hh"

VolumeEnvelop::VolumeEnvelop(const NRX2Proxy& nrx2)
    : Filter(), tick_ (SAMPLE_RATE, 64), nrx2_ (nrx2), enabled_ (false)
{}

int32_t VolumeEnvelop::filter(int32_t freq) {
    if (this->enabled_) {
        if (this->tick_.next())
            this->sweep_--;
        if (this->sweep_ == 0) {
            logging::info("volume sweep.");
            if (this->way_ && this->volume_ != 0xf)
                this->volume_ -= 1;
            else if (!this->way_ && this->volume_ != 0)
                this->volume_ += 1;
            this->sweep_ = this->nrx2_.envelop_sweep.get();
        }
        if (this->volume_ == 0)
            return 0;
        if (this->volume_ <= 15) {
            freq /= 16 - this->volume_;
        }
    }
    return freq;
}

void VolumeEnvelop::reload() {
    this->volume_ = this->nrx2_.volume.get();
    this->way_ = this->nrx2_.envelop_way.get();
    this->sweep_ = this->nrx2_.envelop_sweep.get();
    this->enabled_ = (this->sweep_ != 0);
    logging::info("volume: %d", this->volume_);
    logging::info("sweeps: %d", this->sweep_);
}
