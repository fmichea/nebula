#ifndef MMU_HH_
# define MMU_HH_

# include <fcntl.h>
# include <string.h>
# include <string>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# include "logging.hh"
# include "memory/mbcs/mbc.hh"
# include "memory/mbcs/mbc1.hh"
# include "memory/mbcs/mbc2.hh"
# include "memory/mbcs/mbc3.hh"
# include "memory/mbcs/mbc5.hh"
# include "memory/mbcs/romonly.hh"
# include "memory/registers/lcdc.hh"
# include "memory/registers/palette.hh"
# include "memory/registers/register.hh"
# include "memory/registers/stat.hh"

class MMU
{

public:
    MMU();
    ~MMU();

    bool load_rom(std::string filename);
    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

# define X(Reg, Addr, Value)    \
    Register Reg;
# include "registers.hh"
# undef X
    Register IE;
    STATProxy STAT;
    LCDCProxy LCDC;
    PaletteProxy BGP;
    PaletteProxy OBP[2];

    bool            stopped;

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

# define X(Name, Size)      \
    uint8_t*        Name;
# include "mmap.def"
# undef X
};

# include "mmu.hxx"

#endif // !MMU_HH_
