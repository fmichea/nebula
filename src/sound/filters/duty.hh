#ifndef NEBULA_SOUND_FILTERS_DUTY_HH_
# define NEBULA_SOUND_FILTERS_DUTY_HH_

# include <math.h>
# include <stdint.h>

# include "../../logging.hh"
# include "../../memory/registers/nrx1.hh"
# include "../../memory/registers/nrx4.hh"
# include "../../memory/registers/register.hh"
# include "../../utils/converter.hh"
# include "../../utils/cycle.hh"
# include "../constants.hh"
# include "filter.hh"

class Duty : public Filter {
public:
    Duty(const NRX1Proxy& nrx1);

    void reload(int32_t freq);
    int32_t filter(int32_t frequency);

private:
    const NRX1Proxy& nrx1_;

    Cycle<unsigned int> frame_;
    Cycle<unsigned int> phase_;

    Converter timer_tick_;

    unsigned int pattern_;
};

#endif // !NEBULA_SOUND_FILTERS_DUTY_HH_
