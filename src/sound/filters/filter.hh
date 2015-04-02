#ifndef NEBULA_SOUND_FILTERS_FILTER_HH_
# define NEBULA_SOUND_FILTERS_FILTER_HH_

# include <stdint.h>

class Filter {
public:
    Filter() : enabled_ (false) {}
    virtual ~Filter() {}

    virtual void reload(int32_t freq) = 0;
    virtual int32_t filter(int32_t freq) = 0;

protected:
    bool enabled_;
};

#endif // !NEBULA_SOUND_FILTERS_FILTER_HH_
