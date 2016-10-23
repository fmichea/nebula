#include "interrupts.hh"

#define TIMER_ENABLED(Tac) ((Tac & 0x4) >> 2)
#define TIMER_CYCLES_SELECTOR(Tac) (Tac & 0x3)

static void _div_reg_written_to(void* data, const WatchEvent* UNUSED(event)) {
    Interrupts* interrupts = static_cast<Interrupts*>(data);
    interrupts->div_reg_written_to();
}

static void _tac_reg_written_to(void* data, const WatchEvent* event) {
    Interrupts* interrupts = static_cast<Interrupts*>(data);
    interrupts->tac_reg_written_to(event);
}

Interrupts::Interrupts(MMU* mmu, Z80Registers& regs)
    : mmu_ (mmu)
    , regs_ (regs)
    , _timer_counter (0)
    , _delayed_timer_val (0)
    , tima_reload_scheduled_ (false)
{
    this->mmu_->subscribe(this->mmu_->DIV.addr(), WatchType::WRITE_ONLY, _div_reg_written_to, this);
    this->mmu_->subscribe(this->mmu_->TAC.addr(), WatchType::WRITE_ONLY, _tac_reg_written_to, this);
}

void Interrupts::manage_interrupts() {
    uint8_t ints[] = {0x40, 0x48, 0x50, 0x58, 0x60};

    uint8_t if_flag = this->mmu_->IF.get();
    uint8_t state = mmu_->IE.get() & if_flag;

    if (!(this->regs_.IME || this->regs_.halt_mode) || !state) {
        return;
    }

    if (state && this->regs_.halt_mode) {
        this->regs_.halt_mode = false;
        return;
    }

    for (uint8_t it = 0; it < 5 && state; ++it) {
        if ((state >> it) & 0x1) {
            this->mmu_->IF.set(if_flag & (~(0x1 << it)));
            this->regs_.PC -= 1;
            di(*this->mmu_, this->regs_);
            rst_nn(*this->mmu_, this->regs_, ints[it]);
            break;
        }
    }
}

void Interrupts::div_reg_written_to() {
    this->_timer_counter = 0;
    this->_manage_tima_increment();
}

void Interrupts::tac_reg_written_to(const WatchEvent* UNUSED(event)) {
    this->_manage_tima_increment();
}

void Interrupts::manage_timer(uint8_t cycles) {
    for (uint8_t x = 0; x < cycles; x += 4) {
        uint16_t orig_timer_counter = this->_timer_counter;
        this->_timer_counter += 4;

        // Divider register, inc at a speed of 16384 Hz (every 256 cpu cycles).
        if ((orig_timer_counter & 0xff00) != (this->_timer_counter & 0xff00)) {
            this->mmu_->DIV.set(this->mmu_->DIV.get() + 1);
        }

        if (this->tima_reload_scheduled_) {
            this->mmu_->TIMA.set(this->mmu_->TMA.get());
            this->mmu_->IF.set(this->mmu_->IF.get() | INTERRUPT_TIMER);

            this->tima_reload_scheduled_ = false;
        }

        this->_manage_tima_increment();
    }
}

void Interrupts::_manage_tima_increment() {
    uint8_t tac = this->mmu_->TAC.get();

    // TAC controls the behaviour of the timer, TIMA is the timer itself.
    uint8_t val = TIMER_ENABLED(tac);

    uint16_t timer_counter_mask = this->_cycles_for_timer(tac);
    val &= (this->_timer_counter & timer_counter_mask) ? 1 : 0;

    if (!val && this->_delayed_timer_val) {
        this->mmu_->TIMA.set((this->mmu_->TIMA.get() + 1) & 0xff);
        if (this->mmu_->TIMA.get() == 0) {
            this->tima_reload_scheduled_ = true;
        }
    }
    this->_delayed_timer_val = val;
}

uint16_t Interrupts::_cycles_for_timer(uint8_t tac) {
    switch (TIMER_CYCLES_SELECTOR(tac)) { // Controls speed of timer.
    case 0x0: // 4096 Hz - 1024 cpu cycles
        return 0x0200;
    case 0x1: // 262144 Hz - 16 cpu cycles
        return 0x0008;
    case 0x2: // 65536 Hz - 64 cpu cycles
        return 0x0020;
    case 0x3: // 16384 Hz - 256 cpu cycles
        return 0x0080;
    };
}
