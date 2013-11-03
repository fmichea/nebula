#ifndef KBHANDLER_HH_
# define KBHANDLER_HH_

# include "../../memory/mmu.hh"

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

struct kbh_event {
    e_kbh_key           key;
    e_kbh_event_type    type;
};
typedef struct kbh_event s_kbh_event;

class KBEventHandler {
public:
    KBEventHandler(MMU& mmu);

    void do_cycle();
    void handle_event(s_kbh_event* event);

private:
    MMU& mmu_;

    // state_ and select_ follow the GB convention: 0 = enabled
    uint8_t state_; // b0-3 = directions; b4-7 = buttons
    uint8_t select_; // select lines, b0: directions; b1: buttons
    bool trigger_int_;
};

class KBHandler
{
public:
    KBHandler(KBEventHandler* eventhandler) : eventhandler_ (eventhandler) {
        (void) this->eventhandler_;
    }
    virtual ~KBHandler() {}

protected:
    KBEventHandler* eventhandler_;
};

#endif // !KBHANDLER_HH_
