#ifndef TIMER_HH_
# define TIMER_HH_

# include <stdint.h>
# include <unistd.h>
# include <sys/time.h>

# include "../logging.hh"

# define ELAPSED_FRAME 16742

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

#endif // !TIMER_HH_
