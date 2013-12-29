nebula - GameBoy emulator
=========================

This project implements a GameBoy Color emulator. It is still a bit buggy, but
it does run quite a lot of games now, which is cool!

This is implemented in C++ and has python and SDL2 build dependencies too. To
build it cleanly, proceed as follows:

    $ git clone https://bitbucket.org/kushou/nebula.git
    $ cd nebula
    $ mkdir build-dev
    $ cd build-dev
    $ cmake .. -D_DISPLAY_BACKEND=SDL
    $ make

And then you can execute a ROM with the following command:

    $ ./nebula --rom /path/to/rom

Links
-----

* [Documentation](http://nocash.emubase.de/pandocs.htm)
* [Test ROM](http://blargg.8bitalley.com/parodius/gb-tests/)
