#include "square.hh"

namespace nebula { namespace sound { namespace filters { namespace channels {

Square::Square() : Filter() {}

void Square::reload() {}

int32_t Square::filter(int32_t freq) {
    if (0 <= freq && freq < 2048)
        freq = 131072 / (2048 - freq);
    else
        freq = 0;
    return freq;
}

}}}} /* !namespaces */
