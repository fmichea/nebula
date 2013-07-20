#ifndef KBHANDLER_HH_
# define KBHANDLER_HH_

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

class KBHandler
{
public:
    virtual ~KBHandler() {};

    virtual bool pollEvent(s_kbh_event* event) = 0;
};

#endif // !KBHANDLER_HH_
