#ifndef NEBULA_Z80_INTERRUPTS_HH_
# define NEBULA_Z80_INTERRUPTS_HH_

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"
# include "z80registers.hh"
# include "z80opcodes.hh"

# define INTERRUPT_VBLANK   (1 << 0)
# define INTERRUPT_STAT     (1 << 1)
# define INTERRUPT_TIMER    (1 << 2)
# define INTERRUPT_SERIAL   (1 << 3)
# define INTERRUPT_JOYPAD   (1 << 4)

class Interrupts {
public:
    Interrupts(MMU* mmu, Z80Registers& regs);
    void manage_interrupts();
    void manage_timer(uint8_t cycles);

public: // FIXME: not really public.
    void div_reg_written_to();
    void tac_reg_written_to(const WatchEvent* event);

protected:
    uint16_t _cycles_for_timer(uint8_t tac);
    void _manage_tima_increment();

private:
    MMU*            mmu_;
    Z80Registers&   regs_;

    uint16_t _timer_counter;
    uint8_t _delayed_timer_val;
    bool tima_reload_scheduled_;
};

#endif // !NEBULA_Z80_INTERRUPTS_HH_
