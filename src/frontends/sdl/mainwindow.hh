#ifndef NEBULA_FRONTENDS_SDL_MAINWINDOW_HH_
# define NEBULA_FRONTENDS_SDL_MAINWINDOW_HH_

# include <SDL.h>

# include "../../logging.hh"
# include "../abstractmainwindow.hh"
# include "sdldisplay.hh"

namespace nebula { namespace frontends { namespace sdl {

class MainWindow : public AbstractMainWindow {
public:
    MainWindow(const char* windowName);
    ~MainWindow();

    void run();

private:
    SDL_Window*     window_;
};

}}} /* !nebula::frontends::sdl */

#endif /* !NEBULA_FRONTENDS_SDL_MAINWINDOW_HH_ */
