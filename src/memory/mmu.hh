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

class MMU
{
    class Register
    {
    public:
        Register(uint8_t& reg)
            : reg_ (reg)
        {}

        Register& operator= (uint8_t val)
        {
            this->reg_ = val;
            return *this;
        }

        Register& operator= (Register& val)
        {
            this->reg_ = val.reg_;
            return *this;
        }

        operator uint8_t ()
        {
            return this->reg_;
        }

    private:
        uint8_t&    reg_;
    };

public:
    MMU();
    ~MMU();

    bool load_rom(std::string filename);
    template<typename T> T read(uint16_t addr);
    template<typename T> void write(uint16_t addr, T value);

    Register IE;
    Register LY;
    Register LYC;
    Register NR10;
    Register NR11;
    Register NR12;
    Register NR14;
    Register NR21;
    Register NR22;
    Register NR24;
    Register NR30;
    Register NR31;
    Register NR32;
    Register NR33;
    Register NR41;
    Register NR42;
    Register NR43;
    Register NR44;
    Register NR50;
    Register NR51;
    Register NR52;
    Register SCX;
    Register SCY;
    Register TAC;
    Register TIMA;
    Register TMA;
    Register WX;
    Register WY;

private:
    bool load_mbc(uint8_t ct_type);
    bool load_cartridge_type(uint8_t val);
    bool load_rom_size(uint8_t val);
    bool load_ram_size(uint8_t val);
    bool reset_registers();

    int             fd_;
    size_t          size_;
    char*           rom_;
    MBC*            mbc_;
    std::string     title_;
    std::string     target_;
};

# include "mmu.hxx"

#endif // !MMU_HH_
