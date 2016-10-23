#include "watch.hh"

WatchEvent::WatchEvent(AccessType type, uint16_t addr)
    : _type (type)
    , _addr (addr)
    , _value (0)
    , _prev_value (0)
    , _valid (false)
{}

WatchEvent::~WatchEvent()
{}

void WatchEvent::set_values(uint8_t prev, uint8_t value) {
    this->_valid = true;
    this->_prev_value = prev;
    this->_value = value;
}

bool WatchEvent::valid() const {
    return this->_valid;
}

AccessType WatchEvent::type() const {
    return this->_type;
}

uint16_t WatchEvent::addr() const {
    return this->_addr;
}

uint8_t WatchEvent::value() const {
    return this->_value;
}

uint8_t WatchEvent::previous_value() const {
    return this->_prev_value;
}

Watch::Watch()
    : _events ()
    , _noop_event (new WatchEvent(AccessType::READ, 0))
{
    for (uint32_t idx = 0; idx < GB_ADDR_SPACE; ++idx) {
        this->watchref_lists_[idx] = Watch::WatchRefList();
    }
}

Watch::~Watch()
{
    // Clean-up all of the references for memory watches.
    for (uint32_t idx = 0; idx < GB_ADDR_SPACE; ++idx) {
        for (WatchRef* ref : this->watchref_lists_[idx]) {
            delete ref;
        }
        this->watchref_lists_[idx].clear();
    }
}

void Watch::subscribe(uint16_t addr, WatchType type, WatchHook hook, void* data) {
    // Create the watch ref and fill all the fields.
    WatchRef* ref = new WatchRef;
    ref->type = type;
    ref->hook = hook;
    ref->data = data;
    // Add that watch to the ref list and .
    this->watchref_lists_[addr].push_back(ref);
}


void Watch::unsubscribe(uint16_t addr, WatchHook hook) {
    WatchRefFinder finder(hook);
    this->watchref_lists_[addr].remove_if(finder);
}

WatchEvent* Watch::create_event(AccessType type, uint16_t addr) {
    WatchEvent* event = this->_noop_event;

    if (!this->watchref_lists_[event->addr()].empty()) {
        event = new WatchEvent(type, addr);
        this->_events.push(event);
    }
    return event;
}

inline bool watchref_triggers(const WatchRef* ref, AccessType type) {
    if (ref->type == WatchType::READ_WRITE) {
        return true;
    }

    switch (type) {
    case AccessType::READ:
        return ref->type == WatchType::READ_ONLY;
    case AccessType::WRITE:
        return ref->type == WatchType::WRITE_ONLY;
    };
}

void Watch::trigger_all_events() {
    while (!this->_events.empty()) {
        WatchEvent* event = this->_events.front();
        this->_events.pop();

        if (!event->valid()) {
            goto free_event;
        }

        for (const WatchRef* ref : this->watchref_lists_[event->addr()]) {
            if (watchref_triggers(ref, event->type())) {
                ref->hook(ref->data, event);
            }
        }

free_event:
        delete event;
    }
}
