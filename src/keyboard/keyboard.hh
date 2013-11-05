#ifndef KEYBOARD_HH_
# define KEYBOARD_HH_

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"
# include "../z80/interrupts.hh"
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
    Keyboard(MMU& mmu);
    virtual ~Keyboard();

    void do_cycle();
private:
    KBEventHandler* eventhandler_;
    KBHandler* handler_;
};

#endif // !KEYBOARD_HH_
