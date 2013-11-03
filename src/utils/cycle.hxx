#ifndef CYCLER_HXX_
# define CYCLER_HXX_

# include "cycle.hh"

template <typename T>
Cycle<T>::Cycle(T maximum)
    : counter_ (T(0)), maximum_ (maximum)
{}

template <typename T>
T& Cycle<T>::operator++ () {
    this->counter_ = (this->counter_ + 1);
    if (this->maximum_ <= this->counter_)
        this->counter_ = T(0);
    return this->counter_;
}

template <typename T>
T Cycle<T>::operator++ (int unused) {
    T tmp(this->counter_);

    (void) unused;
    operator++();
    return tmp;
}

template <typename T>
void Cycle<T>::reset() {
    this->counter_ = T(0);
}

#endif // !CYCLE_HXX_
