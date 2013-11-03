#ifndef WAVE_HH_
# define WAVE_HH_

# include <math.h>
# include <stdint.h>

# include "../../logging.hh"
# include "../../memory/registers/nrx1.hh"
# include "../../utils/cycle.hh"
# include "../constants.hh"
# include "filter.hh"

class WaveForm : public Filter {
public:
    WaveForm(const NRX1Proxy& nrx1);

    void reload();
    int32_t filter(int32_t frequency);

private:
    Cycle<unsigned int> frame_;
    const NRX1Proxy& nrx1_;
    unsigned int duty_;
};

#endif // !WAVE_HH_
