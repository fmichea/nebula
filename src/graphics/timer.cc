#include "timer.hh"

Timer::Timer()
{
    this->last_ = this->tick();
}

void Timer::adjust()
{
    uint64_t delta = this->tick() - this->last_;

    logging::debug("Displayed frame in %d milliseconds.", delta);
    if (delta < ELAPSED_FRAME)
    {
        usleep(ELAPSED_FRAME - delta);
    }
    this->last_ = this->tick();
}

uint64_t Timer::tick()
{
    struct timeval current;

    if (gettimeofday(&current, NULL) < 0) {
        logging::error("Couldn't fetch tick.");
        return 0;
    }
    return ((uint64_t) current.tv_sec) * 1000000 + current.tv_usec;
}
