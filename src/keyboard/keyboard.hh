#ifndef KEYBOARD_HH_
# define KEYBOARD_HH_

# include "../z80/interrupts.hh"
# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"

# include "handlers/kbhandler.hh"
# include "handlers/sdlkbhandler.hh"

# define NB_KEYBINDINGS 8

typedef struct {
    e_kbh_key   key;
    uint8_t     bit;
    uint8_t     request;
} s_keybinding;

class Keyboard {
public:
    Keyboard();
    virtual ~Keyboard();

    void do_cycle(MMU& mmu);

private:
    KBHandler* handler_;
    // state_ and select_ follow the GB convention: 0 = enabled
    uint8_t state_; // b0-3 = directions; b4-7 = buttons
    uint8_t select_; // select lines, b0: directions; b1: buttons
};

#endif // !KEYBOARD_HH_
