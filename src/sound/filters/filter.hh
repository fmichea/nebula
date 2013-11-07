#ifndef FILTER_HH_
# define FILTER_HH_

# include <stdint.h>

class Filter {
public:
    Filter() : enabled_ (false) {}
    virtual ~Filter() {}

    virtual void reload() = 0;

    virtual int32_t filter(int32_t frequency) = 0;

protected:
    bool enabled_;
};

#endif // !FILTER_HH_
