#ifndef CONSTANTS_HH_
# define CONSTANTS_HH_

# define PI 3.14159265

# define NB_CHANNELS    2
# define NB_GB_CHANNELS 4

# define SAMPLE_RATE 48000
# define SAMPLE_COUNT 2400

# define BITS 16

# define MAX_FREQ (1 << (BITS - 1)) - 1
# define MIN_FREQ -(1 << (BITS - 1))

#endif // !CONSTANTS_HH_
