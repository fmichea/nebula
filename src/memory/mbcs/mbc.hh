#ifndef NEBULA_MEMORY_MBCS_MBC_HH_
# define NEBULA_MEMORY_MBCS_MBC_HH_

# include <stdint.h>
# include <stdlib.h>

# include "../../logging.hh"
# include "../../utils/utils.hh"

class MBC
{
public:
    MBC(void* rom);
    virtual ~MBC();

    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

protected:
    virtual void* read_address(uint16_t addr);
    virtual void* write_address(uint16_t addr, uint16_t value);

    virtual void* read_rom_address(uint16_t addr);
    virtual void* read_ram_address(uint16_t addr);
    virtual void* write_rom_bank(uint16_t addr, uint16_t value);
    virtual void* write_ram_bank(uint16_t addr, uint16_t value);
    virtual void* write_extra_address(uint16_t addr, uint16_t value);
    virtual void* write_ram_address(uint16_t addr, uint16_t value);

    char* rom_;
    char* ram_;

    uint16_t    rom_bank_;
    uint8_t     ram_bank_;
};

# include "mbc.hxx"

#endif // !NEBULA_MEMORY_MBCS_MBC_HH_
