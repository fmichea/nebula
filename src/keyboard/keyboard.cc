#include "keyboard.hh"

static s_keybinding keybindings[NB_KEYBINDINGS] = {
    // Direction Keys.
    { .key = KBH_KEY_RIGHT, .bit = 0, .request = 4 },
    { .key = KBH_KEY_LEFT, .bit = 1, .request = 4 },
    { .key = KBH_KEY_UP, .bit = 2, .request = 4 },
    { .key = KBH_KEY_DOWN, .bit = 3, .request = 4 },

    // Button Keys.
    { .key = KBH_KEY_A, .bit = 0, .request = 5 }, // B
    { .key = KBH_KEY_B, .bit = 1, .request = 5 }, // A
    { .key = KBH_KEY_SELECT, .bit = 2, .request = 5 }, // Select
    { .key = KBH_KEY_START, .bit = 3, .request = 5 }, // Start
};

Keyboard::Keyboard(MMU& mmu)
    : handler_ (nullptr)
{
    this->eventhandler_ = new KBEventHandler(mmu);
#if _TARGET == 0
    this->handler_ = new SDLKBHandler(this->eventhandler_);
#endif
}

Keyboard::~Keyboard()
{
    delete this->handler_;
    delete this->eventhandler_;
}

void Keyboard::do_cycle()
{
    this->eventhandler_->do_cycle();
}

KBEventHandler::KBEventHandler(MMU& mmu)
    : mmu_ (mmu), state_(0xFF), select_(0x03), trigger_int_ (false)
{}

void KBEventHandler::do_cycle()
{
    uint8_t joyp = 0xF;

    // update internal state
    this->select_ = (this->mmu_.read<uint8_t>(0xFF00) >> 4) & 0x3;

    // write the proper value in JOYP
    if ((this->select_ & 0x1) == 0)
        joyp &= this->state_;
    if (((this->select_ >> 1) & 0x1) == 0)
        joyp &= this->state_ >> 4;
    // 0xC0: b6, b7 = 1 for GB (0 for SGB)
    this->mmu_.write<uint8_t>(0xFF00, 0xC0 | (this->select_ << 4) | joyp);

    // Keyboard interrupt requested when key pressed.
    if (this->trigger_int_) {
        this->mmu_.IF.set(this->mmu_.IF.get() | INTERRUPT_JOYPAD);
        this->trigger_int_ = false;
    }
}

void KBEventHandler::handle_event(s_kbh_event* event)
{
    if (event->type == KBH_EVENT_TYPE_KEYDOWN && event->key == KBH_KEY_QUIT) {
        this->mmu_.stopped = true;
    } else {
        for (int it = 0; it < NB_KEYBINDINGS; it++)
        {
            if (event->key == keybindings[it].key)
            {
                BitProxy proxy(&this->state_, it, 0x1);
                proxy.set(event->type == KBH_EVENT_TYPE_KEYDOWN ? 0 : 1);

                if (event->type == KBH_EVENT_TYPE_KEYDOWN)
                    this->trigger_int_ = true;
            }
        }
    }
}
