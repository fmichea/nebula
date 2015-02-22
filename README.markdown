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

Write your own emulator
-----------------------

Here are some good links to documentation that would help you understand the
inner workings of a GameBoy and write your own emulator.

 - General purpose documentation: [1][general_doc1]
 - Sound system: [1][sound_doc1], [2][sound_doc2]

Testing the thing:

 - Test ROM: [1][test_roms1]


[general_doc]: http://nocash.emubase.de/pandocs.htm
[sound_doc1]: http://gbdev.gg8.se/wiki/articles/Gameboy_sound_hardware
[test_roms1]: http://blargg.8bitalley.com/parodius/gb-tests/
