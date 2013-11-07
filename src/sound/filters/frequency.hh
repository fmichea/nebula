#ifndef FREQUENCY_HH_
# define FREQUENCY_HH_

# include "filter.hh"

class QuandrangularChannel : public Filter {
public:
    QuandrangularChannel();

    void reload();
    int32_t filter(int32_t freq);
};

#endif /* !FREQUENCY_HH_ */
