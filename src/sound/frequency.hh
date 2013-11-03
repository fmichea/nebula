#ifndef FREQUENCY_HH_
# define FREQUENCY_HH_

# include <unistd.h>
# include <stdint.h>
# include <cmath>

# include "../logging.hh"
# include "constants.hh"

typedef enum {
    WAVEFORM_SINE,
    WAVEFORM_SQUARE,
} waveform_type;

class WaveForm {
public:
    WaveForm(waveform_type type);

    unsigned int frequency() const;
    void set_frequency(unsigned int freq);

    int16_t next();

private:
    waveform_type type_;
    unsigned int counter_;
    unsigned int cur_freq_;
};

class Converter {
public:
    Converter(unsigned int base_rate, unsigned int final_rate);

    bool next();
    void reset();

private:
    unsigned int counter_;
    unsigned int divider_;
};

#endif // !FREQUENCY_HH_
