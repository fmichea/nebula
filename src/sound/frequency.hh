#ifndef FREQUENCY_HH_
# define FREQUENCY_HH_

# include <unistd.h>
# include <stdint.h>
# include <cmath>

# include "../logging.hh"
# include "constants.hh"

class Converter {
public:
    Converter(unsigned int base_rate, unsigned int final_rate);

    bool next();
    void reset();

    void set_final_rate(unsigned int final_rate);

private:
    unsigned int counter_;
    unsigned int base_rate_;
    unsigned int divider_;
};

#endif // !FREQUENCY_HH_
