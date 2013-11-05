#ifndef MMU_HH_
# define MMU_HH_

# include <fcntl.h>
# include <string.h>
# include <string>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# include "../logging.hh"

# include "mbcs/mbc.hh"
# include "mbcs/mbc1.hh"
# include "mbcs/mbc2.hh"
# include "mbcs/mbc3.hh"
# include "mbcs/mbc5.hh"
# include "mbcs/romonly.hh"

# include "registers/lcdc.hh"
# include "registers/nr10.hh"
# include "registers/nrx1.hh"
# include "registers/nrx2.hh"
# include "registers/nrx4.hh"
# include "registers/nr52.hh"
# include "registers/palette.hh"
# include "registers/register.hh"
# include "registers/stat.hh"

enum class GBType { GB, CGB, SGB };

class MMU
{

public:
    MMU();
    ~MMU();

    bool load_rom(std::string filename);
    void do_hdma();
    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

# define X2(RegType, Reg, Addr, Value) RegType Reg;
# define X1(Reg, Addr, Value) X2(RegisterProxy, Reg, Addr, Value)
# include "registers.def"
# undef X1
# undef X2
    RegisterProxy IE;

    bool            stopped;
    GBType          gb_type = GBType::GB;
    bool            hdma_active = false;

private:
    bool load_mbc(uint8_t ct_type);
    bool load_rom_size(uint8_t val);
    bool load_ram_size(uint8_t val);
    bool reset_registers();

    char            title_[0x18];
    int             fd_;
    MBC*            mbc_;
    size_t          size_;
    std::string     target_;
    uint16_t        hdma_index_ = 0;
    uint16_t        hdma_length_ = 0;

# define X(Name, Size)      \
    uint8_t*        Name;
# include "mmap.def"
# undef X

    // GPU needs to access video memory directly
    friend class GPU;
};

# include "mmu.hxx"

#endif // !MMU_HH_
