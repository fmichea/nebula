#ifndef NEBULA_SOUND_FILTERS_CHANNELS_SQUARE_HH_
# define NEBULA_SOUND_FILTERS_CHANNELS_SQUARE_HH_

# include "../filter.hh"
# include "../../../utils/utils.hh"

namespace nebula { namespace sound { namespace filters { namespace channels {

class Square : public Filter {
public:
    Square();

    void reload(int32_t freq);
    int32_t filter(int32_t freq);
};

}}}} /* !namespaces */

#endif /* !NEBULA_SOUND_FILTERS_CHANNELS_SQUARE_HH_ */
