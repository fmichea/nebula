#ifndef NEBULA_SOUND_FILTERS_CHANNELS_WAVE_HH_
# define NEBULA_SOUND_FILTERS_CHANNELS_WAVE_HH_

# include "../../../memory/mmu.hh"
# include "../../../utils/utils.hh"
# include "../../../utils/cycle.hh"
# include "../../../utils/converter.hh"
# include "../../constants.hh"
# include "../filter.hh"

namespace nebula { namespace sound { namespace filters { namespace channels {

class Wave : public Filter {
public:
    Wave(MMU* mmu);

    void reload(int32_t freq);
    int32_t filter(int32_t freq);

private:
    Cycle<uint8_t> sample_;
    Converter timer_tick_;
    MMU* mmu_;
};

}}}} /* !namespaces */

#endif /* !NEBULA_SOUND_FILTERS_CHANNELS_WAVE_HH_ */
