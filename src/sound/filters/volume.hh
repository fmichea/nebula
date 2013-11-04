#ifndef VOLUME_HH_
# define VOLUME_HH_

# include <limits>

# include "../../memory/registers/nrx2.hh"
# include "../frequency.hh"
# include "filter.hh"

class VolumeEnvelop : public Filter {
public:
    VolumeEnvelop(const NRX2Proxy& nrx2);

    void reload();
    int32_t filter(int32_t freq);

private:
    Converter tick_;
    const NRX2Proxy& nrx2_;

    uint8_t volume_;
    uint8_t way_;
    uint8_t sweep_;
};

#endif // !VOLUME_HH_
