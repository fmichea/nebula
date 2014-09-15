#ifndef NEBULA_FRONTENDS_ABSTRACTDISPLAY_HH_
# define NEBULA_FRONTENDS_ABSTRACTDISPLAY_HH_

# include <SDL.h>
# include <pthread.h>

# define WIDTH  160
# define HEIGHT 144

namespace nebula { namespace frontends {

class AbstractDisplay {
public:
    virtual ~AbstractDisplay() {}

    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void commit() = 0;
    virtual void refresh() = 0;

    virtual uint32_t getColor(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void setPixel(uint8_t x, uint8_t y, uint32_t color) = 0;
};

}} /* !namespace nebula::frontends */

#endif /* !NEBULA_FRONTENDS_ABSTRACTDISPLAY_HH_ */
