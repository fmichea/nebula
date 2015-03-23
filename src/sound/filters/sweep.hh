#ifndef SWEEP_HH_
# define SWEEP_HH_

# include "../../memory/registers/nr10.hh"
# include "../../memory/registers/nr52.hh"
# include "../../utils/converter.hh"
# include "../../utils/cycle.hh"
# include "../constants.hh"
# include "filter.hh"

typedef enum {
    FREQUENCYSWEEP_NEGATE_TRUE,
    FREQUENCYSWEEP_NEGATE_FALSE,
} frequencysweep_negate;

class FrequencySweep : public Filter {
public:
    FrequencySweep(int num, NR52Proxy& nr52, const NR10Proxy& nr10);

    void reload();
    int32_t filter(int32_t freq);

private:
    int num_;
    NR52Proxy& nr52_;

    const NR10Proxy& nr10_;
    Converter tick_;
    Cycle<unsigned int> counter_;

    frequencysweep_negate negate_;
    int32_t shift_;

    int32_t diff_;
};

#endif /* !SWEEP_HH_ */
