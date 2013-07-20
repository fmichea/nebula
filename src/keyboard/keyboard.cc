#include "keyboard.hh"

static s_keybinding keybindings[NB_KEYBINDINGS] = {
    // Direction Keys.
    { .key = KBH_KEY_DOWN, .bit = 3, .request = 4 },
    { .key = KBH_KEY_UP, .bit = 2, .request = 4 },
    { .key = KBH_KEY_LEFT, .bit = 1, .request = 4 },
    { .key = KBH_KEY_RIGHT, .bit = 0, .request = 4 },

    // Button Keys.
    { .key = KBH_KEY_START, .bit = 3, .request = 5 }, // Start
    { .key = KBH_KEY_SELECT, .bit = 2, .request = 5 }, // Select
    { .key = KBH_KEY_B, .bit = 1, .request = 5 }, // A
    { .key = KBH_KEY_A, .bit = 0, .request = 5 }, // B
};

Keyboard::Keyboard()
    : handler_ (nullptr)
{
#if _TARGET == 0
    this->handler_ = new SDLKBHandler();
#endif
}

Keyboard::~Keyboard()
{
    delete this->handler_;
}

void Keyboard::do_cycle(MMU& mmu)
{
    s_kbh_event event;
    uint8_t joyp;
    bool joyp_changed = false;

    joyp = mmu.read<uint8_t>(0xFF00);
    while (this->handler_->pollEvent(&event)) {
        if (event.type == KBH_EVENT_TYPE_KEYDOWN && event.key == KBH_KEY_QUIT) {
            mmu.stopped = true;
        } else {
            for (int it = 0; it < NB_KEYBINDINGS; it++)
            {
                if (((joyp >> keybindings[it].request) & 0x1) &&
                    event.key == keybindings[it].key)
                {
                    BitProxy proxy(&joyp, keybindings[it].bit, 0x1);
                    proxy.set(event.type == KBH_EVENT_TYPE_KEYDOWN ? 0 : 1);

                    BitProxy req(&joyp, keybindings[it].request, 0x1);
                    req.set(0);

                    joyp_changed = true;
                }
            }
        }
    }
    mmu.write<uint8_t>(0xFF00, joyp);

    // Keyboard interrupt requested when key pressed.
    if (joyp_changed)
        mmu.IF.set(mmu.IF.get() | 0x10);
}
