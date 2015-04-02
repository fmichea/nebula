#ifndef NEBULA_SOUND_CHANNEL_HH_
# define NEBULA_SOUND_CHANNEL_HH_

# include <list>
# include <cmath>

# include "../memory/mmu.hh"
# include "../memory/registers/nrx4.hh"
# include "../logging.hh"
# include "constants.hh"
# include "filters/filter.hh"
# include "../utils/utils.hh"

class Channel {
public:
    Channel(MMU* mmu, int num, const RegisterProxy& nrx3,
            const NRX4Proxy& nrx4, const std::list<Filter*>& filters);
    virtual ~Channel();

    void reload();
    void fill_stream(int16_t* stream, unsigned int len);

protected:
    int num_;
    int32_t frequency_;
    const std::list<Filter*> filters_;

    NR52Proxy& nr52_;
    const RegisterProxy& nrx3_;
    const NRX4Proxy& nrx4_;
};

#endif // !NEBULA_SOUND_CHANNEL_HH_
