#ifndef NEBULA_SOUND_FILTERS_LENGTH_HH_
# define NEBULA_SOUND_FILTERS_LENGTH_HH_

# include <stdint.h>

# include "../../memory/mmu.hh"
# include "../../memory/registers/nr52.hh"
# include "../../memory/registers/nrx1.hh"
# include "../../memory/registers/nrx4.hh"
# include "../../utils/converter.hh"
# include "../../utils/utils.hh"
# include "filter.hh"

class Length : public Filter {
public:
    Length(int num, MMU* mmu, NR52Proxy& nr52, const NRX1Proxy& nrx1, const NRX4Proxy& nrx4);

    void reload(int32_t freq);
    int32_t filter(int32_t frequency);

    void reload_counter();

private:
    Converter tick_;

    uint16_t length_;

    int num_;
    NR52Proxy& nr52_;
    const NRX1Proxy& nrx1_;
    const NRX4Proxy& nrx4_;
};

#endif // !NEBULA_SOUND_FILTERS_LENGTH_HH_
