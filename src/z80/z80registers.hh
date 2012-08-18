#ifndef Z__REGISTERS_HH_
# define Z__REGISTERS_HH_

# include <stdint.h>

# include "../memory/registers/bitproxy.hh"

class WordRegProxy
{
public:
    WordRegProxy(uint8_t& msb, uint8_t& lsb)
        : msb_ (msb), lsb_ (lsb), lsb_mask_ (0xff)
    {}

    WordRegProxy(uint8_t& msb, uint8_t& lsb, uint8_t mask)
        : msb_ (msb), lsb_ (lsb), lsb_mask_ (mask)
    {}

    void set(uint16_t val)
    {
        this->msb_ = (val >> 8) & 0xff;
        this->lsb_ = val & this->lsb_mask_;
    }

    uint16_t get()
    {
        return (this->msb_ << 8) + this->lsb_;
    }

private:
    uint8_t&    msb_;
    uint8_t&    lsb_;
    uint8_t     lsb_mask_;
};

class Z80Registers
{
public:
    class Z80Flags
    {
    public:
        Z80Flags(uint8_t* reg)
            : zf (reg, 7, 1), n (reg, 6, 1), h (reg, 5, 1), cy (reg, 4, 1),
              reg_ (reg)
        {}

        void set(uint8_t val)
        {
            *this->reg_ = (val & 0xf0);
        }

        BitProxy zf;
        BitProxy n;
        BitProxy h;
        BitProxy cy;

    private:
        uint8_t*    reg_;
    };

    Z80Registers()
        : F (&flags_), AF (A, flags_, 0xf0), BC (B, C), DE (D, E), HL (H, L)
    {
        this->AF.set(0x11b0);
        this->BC.set(0x0013);
        this->DE.set(0x00d8);
        this->HL.set(0x014d);
        this->SP = 0xfffe;
        this->PC = 0x0100;

        // Other flags.
        this->IME = false;
        this->halt_mode = false;
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

    bool            IME; // Interrupt Master Enable Flag (Write Only)
    bool            halt_mode; // Waits an interrupt.

private:
    uint8_t flags_;
};

#endif // !Z__REGISTERS_HH_
