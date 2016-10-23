#pragma once
#ifndef NEBULA_MEMORY_WATCH_HH_
# define NEBULA_MEMORY_WATCH_HH_

# include <stdint.h>

# include <list>
# include <queue>

# include "access.hh"

# define GB_ADDR_SPACE 0x10000

enum class WatchType { READ_ONLY, WRITE_ONLY, READ_WRITE };

class WatchEvent {
public:
    WatchEvent(AccessType type, uint16_t addr);
    virtual ~WatchEvent();

    void set_values(uint8_t prev, uint8_t value);

    bool valid() const;

    AccessType type() const;
    uint16_t addr() const;
    uint8_t value() const;
    uint8_t previous_value() const;

private:
    AccessType  _type;
    uint16_t    _addr;
    uint8_t     _value;
    uint8_t     _prev_value;
    bool        _valid;
};

/**
 * parameters:
 *     void*: user provided data to subscribe function.
 *     const WatchEvent*: watch event that was triggered.
 */
typedef void (*WatchHook)(void*, const WatchEvent*);

// Type to store hooks on events, this is used to keep track of them and
// allow easy subscribe/unsubscribe. Note that subscribe/unsubscribe are
// not optimized for being fast yet.
struct WatchRef  {
    WatchType type;
    WatchHook hook;
    void* data;
};

class Watch {
public:
    Watch();
    virtual ~Watch();

    void subscribe(uint16_t addr, WatchType type, WatchHook hook, void* data);
    void unsubscribe(uint16_t addr, WatchHook hook);

    WatchEvent* create_event(AccessType type, uint16_t addr);
    void trigger_all_events();

private:
    class WatchRefFinder {
    public:
        WatchRefFinder(WatchHook hook)
            : hook_ (hook)
        {}

        bool operator () (const WatchRef* ref) {
            return ref->hook == this->hook_;
        }

    private:
        WatchHook hook_;
    };

    typedef std::list<WatchRef*> WatchRefList;

private:
    WatchRefList watchref_lists_[GB_ADDR_SPACE];

    std::queue<WatchEvent*> _events;

    WatchEvent* _noop_event;
};

#endif /* !NEBULA_MEMORY_WATCH_HH_ */
