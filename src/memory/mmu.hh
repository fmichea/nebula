#ifndef MMU_HH_
# define MMU_HH_

# include <string>
# include <string.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>

# include "mbcs/mbc.hh"
# include "mbcs/mbc1.hh"
# include "mbcs/romonly.hh"
# include "../logging.hh"

class MMU
{
    class Register
    {
    public:
        Register()
            : reg_ (0)
        {}

        Register(uint8_t* reg)
            : reg_ (reg)
        {}

        Register(Register& other)
            : reg_ (other.reg_)
        {}

        Register& operator= (uint8_t val)
        {
            *this->reg_ = val;
            return *this;
        }

        operator uint8_t ()
        {
            return (*this->reg_);
        }

    private:
        uint8_t*    reg_;
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

private:
    bool load_mbc(uint8_t ct_type);
    bool load_cartridge_type(uint8_t val);
    bool load_rom_size(uint8_t val);
    bool load_ram_size(uint8_t val);
    bool reset_registers();

    int             fd_;
    size_t          size_;
    uint8_t*        rom_;
    MBC*            mbc_;
    char            title_[0x15];
    std::string     target_;
};

# include "mmu.hxx"

#endif // !MMU_HH_
