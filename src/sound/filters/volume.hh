#ifndef NEBULA_SOUND_FILTERS_VOLUME_HH_
# define NEBULA_SOUND_FILTERS_VOLUME_HH_

# include <limits>

# include "../../memory/registers/nrx2.hh"
# include "../../utils/converter.hh"
# include "../../utils/cycle.hh"
# include "../constants.hh"
# include "filter.hh"

typedef enum {
    VOLUMEENVELOP_INC,
    VOLUMEENVELOP_DEC,
} volumeenvelop_way;

class VolumeEnvelop : public Filter {
public:
    VolumeEnvelop(const NRX2Proxy& nrx2);

    void reload();
    int32_t filter(int32_t freq);

private:
    Converter tick_;
    const NRX2Proxy& nrx2_;

    int32_t volume_;
    volumeenvelop_way way_;
    Cycle<unsigned int> sweep_;
};

#endif // !NEBULA_SOUND_FILTERS_VOLUME_HH_
