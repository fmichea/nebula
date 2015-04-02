#ifndef NEBULA_GRAPHICS_TIMER_HH_
# define NEBULA_GRAPHICS_TIMER_HH_

# include <stdint.h>
# include <unistd.h>
# include <sys/time.h>

# include "../logging.hh"

# ifndef BOOSTER
#  define BOOSTER 100
#endif
# define ELAPSED_FRAME static_cast<uint64_t>((16742 * 100) / BOOSTER)

class Timer
{
public:
    Timer();

    void adjust();

private:
    uint64_t tick();

private:
    uint64_t last_;
};

#endif // !NEBULA_GRAPHICS_TIMER_HH_
