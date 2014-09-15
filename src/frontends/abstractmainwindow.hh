#ifndef NEBULA_FRONTENDS_ABSTRACTMAINWINDOW_HH_
# define NEBULA_FRONTENDS_ABSTRACTMAINWINDOW_HH_

# include <pthread.h>
# include <string>

# include "../graphics/gpu.hh"
# include "../keyboard/keyboard.hh"
# include "../memory/mmu.hh"
# include "../z80/z80.hh"
# include "abstractdisplay.hh"

namespace nebula { namespace frontends {

class AbstractMainWindow {
public:
    AbstractMainWindow();
    virtual ~AbstractMainWindow();

    void execute(std::string romname);

    void gbrun();

protected:
    virtual void run() = 0;

protected:
    AbstractDisplay* display_;

    MMU* mmu_;
    GPU* gpu_;
    Keyboard* kb_;
    Z80* z80_;
};

}} /* !namespace nebula::frontends */

#endif /* !NEBULA_FRONTENDS_ABSTRACTMAINWINDOW_HH_ */
