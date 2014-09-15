#include <iostream>
#include "z80/z80.hh"
#include "commandline.hh"

#if _TARGET == 0
# include "frontends/sdl/mainwindow.hh"
# define NAMESPACE sdl
#endif

int main(int argc, const char** argv)
{
    std::string rom;
    if (!cmdline::parse(rom, argc, argv))
        return 1;
    nebula::frontends::NAMESPACE::MainWindow mw("nebula");
    mw.execute(rom);
    return 0;
}
