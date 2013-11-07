#include "volume.hh"

VolumeEnvelop::VolumeEnvelop(const NRX2Proxy& nrx2)
    : Filter(), tick_ (SAMPLE_RATE, 64), nrx2_ (nrx2), sweep_(0)
{}

int32_t VolumeEnvelop::filter(int32_t freq) {
    if (this->enabled_) {
        if (this->tick_.next() && (--this->sweep_) == 0) {
            switch (this->way_) {
            case VOLUMEENVELOP_INC:
                if (this->volume_ != 0xf)
                    this->volume_++;
                break;
            case VOLUMEENVELOP_DEC:
                if (this->volume_ != 0)
                    this->volume_--;
                break;
            };
//            logging::info("volume sweep: %d", this->volume_);
        }
    }
    freq *= this->volume_;
    freq /= 15;
    return freq;
}

void VolumeEnvelop::reload() {
    this->tick_.reset();

    this->volume_ = this->nrx2_.volume.get();
    this->way_ = (this->nrx2_.envelop_way.get() ? VOLUMEENVELOP_INC : VOLUMEENVELOP_DEC);

    this->sweep_ = Cycle<unsigned int>(this->nrx2_.envelop_sweep.get());
    this->enabled_ = (this->sweep_.maximum() != 0);
//    logging::info("volume: %d", this->volume_);
//    logging::info("volume sweeps: %d (%s)", this->sweep_.maximum(),
//                  this->enabled_ ? "ON" : "OFF");
}
