#ifndef Z__REGISTERS_HH_
# define Z__REGISTERS_HH_

# include <stdint.h>

class WordRegProxy
{
public:
    WordRegProxy(uint8_t& msb, uint8_t& lsb)
        : msb_ (msb), lsb_ (lsb)
    {}

    WordRegProxy& operator= (uint16_t val)
    {
        this->msb_ = (val >> 8) & 0xff;
        this->lsb_ = val & 0xff;
        return *this;
    }

    operator uint16_t ()
    {
        return (this->msb_ << 8) + this->lsb_;
    }

private:
    uint8_t&    msb_;
    uint8_t&    lsb_;
};

class Z80Registers
{
public:
    class Z80Flags
    {
    public:
        class Z80Flag
        {
        public:
            Z80Flag(uint8_t& reg, uint8_t bit)
                : reg_ (reg), bit_ (bit)
            {}

            Z80Flag& operator= (uint8_t val)
            {
                uint8_t bit = (val & 0x1) << this->bit_;

                if (bit) this->reg_ |= bit;
                else this->reg_ &= (~0 ^ bit);
                return *this;
            }

            operator uint8_t ()
            {
                return (this->reg_ >> this->bit_) & 0x1;
            }

        private:
            uint8_t&    reg_;
            uint8_t     bit_;
        };

        Z80Flags(uint8_t& reg)
            : zf (reg, 7), n (reg, 6), h (reg, 5), cy (reg, 4), reg_ (reg)
        {}

        Z80Flags& operator= (uint8_t val)
        {
            this->reg_ = val;
            return *this;
        }

        Z80Flag zf;
        Z80Flag n;
        Z80Flag h;
        Z80Flag cy;

    private:
        uint8_t&    reg_;
    };


    Z80Registers()
        : F (flags_), AF (A, flags_), BC (B, C), DE (D, E), HL (H, L)
    {
        this->AF = 0x11b0;
        this->BC = 0x0013;
        this->DE = 0x00d8;
        this->HL = 0x0140;
        this->SP = 0xfffe;
        this->PC = 0x0100;
    }

    uint8_t         A;
    Z80Flags        F;
    uint8_t         B;
    uint8_t         C;
    uint8_t         D;
    uint8_t         E;
    uint8_t         H;
    uint8_t         L;
    WordRegProxy    AF;
    WordRegProxy    BC;
    WordRegProxy    DE;
    WordRegProxy    HL;
    uint16_t        PC;
    uint16_t        SP;

private:
    uint8_t flags_;
};

#endif // !Z__REGISTERS_HH_
