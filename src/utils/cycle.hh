#ifndef CYCLE_HH_
# define CYCLE_HH_

# include "utils.hh"

template <typename T>
class Cycle {
public:
    Cycle(T maximum);

    T& operator++ ();
    T operator++ (int unused);

    T& operator-- ();
    T operator-- (int unused);

    T counter();
    T maximum();

    void reset();

private:
    T counter_;
    T maximum_;
};

# include "cycle.hxx"

#endif // !CYCLE_HH_
