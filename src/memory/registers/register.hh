#ifndef REGISTER_HH_
# define REGISTER_HH_

class RegisterProxy {
public:
    RegisterProxy() : reg_ (nullptr), addr_ (0) {}

    RegisterProxy(uint8_t* reg, uint16_t addr)
        : reg_ (reg), addr_ (addr)
    {}

    void set(uint8_t val) {
        if (this->reg_ == nullptr)
            return;
        *this->reg_ = val;
    }

    uint8_t get() const {
        if (this->reg_ == nullptr)
            return 0;
        return *this->reg_;
    }

    uint16_t addr() const { return this->addr_; }

private:
    uint8_t*    reg_;
    uint16_t    addr_;
};

#endif // !REGISTER_HH_
