#ifndef NEBULA_UTILS_CYCLE_HXX_
# define NEBULA_UTILS_CYCLE_HXX_

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
T Cycle<T>::operator++ (int UNUSED(unused)) {
    T tmp(this->counter_);

    operator++();
    return tmp;
}

template <typename T>
T& Cycle<T>::operator-- () {
    if (this->counter_ == T(0))
        this->counter_ = this->maximum_;
    this->counter_ = (this->counter_ - 1);
    return this->counter_;
}

template <typename T>
T Cycle<T>::operator-- (int UNUSED(unused)) {
    T tmp(this->counter_);

    operator--();
    return tmp;
}

template <typename T>
void Cycle<T>::reset() {
    this->counter_ = T(0);
}

template <typename T>
T Cycle<T>::counter() {
    return this->counter_;
}

template <typename T>
T Cycle<T>::maximum() {
    return this->maximum_;
}

#endif // !NEBULA_UTILS_CYCLE_HXX_
