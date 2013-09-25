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
# include "registers/palette.hh"
# include "registers/stat.hh"

class MMU
{
    class Register
    {
    public:
        Register()
            : reg_ (0)
        {}

        Register(uint8_t* reg, uint16_t addr)
            : reg_ (reg), addr_ (addr)
        {}

        void set(uint8_t val)
        {
            *this->reg_ = val;
        }

        uint8_t get()
        {
            return *this->reg_;
        }

        uint16_t addr()
        {
            return this->addr_;
        }

    private:
        uint8_t*    reg_;
        uint16_t    addr_;
    };

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
    uint8_t*        rom_;
    uint8_t*        vram_;
    uint8_t*        wram_;
    uint8_t*        oam_;
    uint8_t*        io_;
    uint8_t*        hram_;
};

# include "mmu.hxx"

#endif // !MMU_HH_
