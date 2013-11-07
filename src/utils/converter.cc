#include "converter.hh"

Converter::Converter(unsigned int base_rate, unsigned int final_rate)
    : counter_ (0), base_rate_ (base_rate)
{
    this->set_final_rate(final_rate);
}

bool Converter::next() {
    this->counter_ = (this->counter_ + 1) % this->divider_;
    return this->counter_ == 0;
}

void Converter::set_final_rate(unsigned int final_rate) {
    if (this->base_rate_ < final_rate) {
        logging::error("Should not create a divider with base rate greater "
                       "than final rate");
    }
    this->divider_ = this->base_rate_ / final_rate;
    this->reset();
}

void Converter::reset() {
    this->counter_ = 0;
}
