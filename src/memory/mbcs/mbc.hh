#ifndef NEBULA_MEMORY_MBCS_MBC_HH_
# define NEBULA_MEMORY_MBCS_MBC_HH_

# include <stdint.h>
# include <stdlib.h>

# include "../segments.hh"
# include "../watch.hh"
# include "../../logging.hh"
# include "../../utils/utils.hh"

class MBC {
public:
    MBC();
    virtual ~MBC();

    virtual uint8_t* real_byte_ptr(AccessType type, uint16_t addr, uint8_t value);

protected:
    virtual uint8_t* read_ram_address(uint16_t addr);
    virtual uint8_t* write_ram_address(uint16_t addr, uint8_t value);

    virtual void bank_selector_zone1(uint16_t addr, uint8_t value);
    virtual void bank_selector_zone2(uint8_t value);
    virtual void bank_mode_select(uint8_t value);
};

#endif // !NEBULA_MEMORY_MBCS_MBC_HH_
