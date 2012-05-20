#ifndef MBC_HH_
# define MBC_HH_

# include <stdint.h>
# include <malloc.h>

# include "../../logging.hh"

class MBC
{
public:
    MBC(void* rom);
    virtual ~MBC();

    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

protected:
    virtual void* read_address(uint16_t addr) = 0;
    virtual void* write_address(uint16_t addr, uint16_t value) = 0;

    char* rom_;
    char* ram_;
};

# include "mbc.hxx"

#endif // !MBC_HH_
