#ifndef DISPLAY_HH_
# define DISPLAY_HH_

# define WIDTH 160
# define HEIGHT 144

class Display
{
public:
    virtual ~Display() {};

    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void commit() = 0;

    virtual int setPixel(uint8_t x, uint8_t y, uint32_t color) = 0;

    virtual uint32_t getColor(uint8_t r, uint8_t g, uint8_t b) = 0;
};

#endif // !DISPLAY_HH_
