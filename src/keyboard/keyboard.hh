#ifndef KEYBOARD_HH_
# define KEYBOARD_HH_

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"
# include "../z80/interrupts.hh"

# define NB_KEYBINDINGS 8

enum kbh_event_type {
    KBH_EVENT_TYPE_KEYDOWN,
    KBH_EVENT_TYPE_KEYUP,
};
typedef enum kbh_event_type e_kbh_event_type;

enum kbh_key {
    KBH_KEY_QUIT,

    KBH_KEY_UP,
    KBH_KEY_RIGHT,
    KBH_KEY_DOWN,
    KBH_KEY_LEFT,

    KBH_KEY_A,
    KBH_KEY_B,

    KBH_KEY_START,
    KBH_KEY_SELECT,
};
typedef enum kbh_key e_kbh_key;

typedef struct {
    e_kbh_key   key;
    uint8_t     bit;
    uint8_t     request;
} s_keybinding;

struct kbh_event {
    e_kbh_key           key;
    e_kbh_event_type    type;
};
typedef struct kbh_event s_kbh_event;

class Keyboard {
public:
    Keyboard(MMU* mmu);
    virtual ~Keyboard();

    void do_cycle();
    void handle_event(s_kbh_event* event);

private:
    MMU* mmu_;

    // state_ and select_ follow the GB convention: 0 = enabled
    uint8_t state_; // b0-3 = directions; b4-7 = buttons
    uint8_t select_; // select lines, b0: directions; b1: buttons
    bool trigger_int_;
};

#endif // !KEYBOARD_HH_
