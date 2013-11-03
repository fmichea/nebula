#ifndef CHANNEL_HH_
# define CHANNEL_HH_

# include <cmath>

# include "../../memory/mmu.hh"
# include "../constants.hh"

class Channel {
public:
    Channel(MMU& mmu);
    virtual ~Channel();

    virtual void fill_stream(int16_t* stream, int len) = 0;

protected:
    MMU& mmu_;
};

#endif // !CHANNEL_HH_
