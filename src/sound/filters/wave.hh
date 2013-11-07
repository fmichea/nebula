#ifndef WAVE_HH_
# define WAVE_HH_

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

class WaveForm : public Filter {
public:
    WaveForm(const NRX1Proxy& nrx1, const RegisterProxy& nrx3, const NRX4Proxy& nrx4);

    void reload();
    int32_t filter(int32_t frequency);

private:
    Cycle<unsigned int> frame_;
    const NRX1Proxy& nrx1_;
    const RegisterProxy& nrx3_;
    const NRX4Proxy& nrx4_;

    Converter duty_tick_;
    Cycle<unsigned int> duty_phase_;
    bool duty_reload_;
    unsigned int duty_pattern_;
};

#endif // !WAVE_HH_
