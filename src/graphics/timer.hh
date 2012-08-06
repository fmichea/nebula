#ifndef TIMER_HH_
# define TIMER_HH_

# include <SDL/SDL.h>
# include "../logging.hh"

# define ELAPSED_FRAME 16

class Timer
{
public:
    Timer();

    void adjust();

private:
    unsigned int last_;
};

#endif // !TIMER_HH_
