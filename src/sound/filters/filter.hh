#ifndef FILTER_HH_
# define FILTER_HH_

class Filter {
public:
    Filter() {}
    virtual ~Filter() {}

    virtual void reload() = 0;

    virtual int32_t filter(int32_t frequency) = 0;
};

#endif // !FILTER_HH_
