#ifndef KEYBOARD_HH_
# define KEYBOARD_HH_

// FIXME: # include <SDL/SDL.h>

# include "../memory/mmu.hh"
# include "../memory/registers/bitproxy.hh"

# define NB_KEYBINDINGS 8

typedef struct {
    // FIXME: SDLKey      key;
    uint8_t     bit;
    uint8_t     request;
} s_keybinding;

void keyboard_cycle(MMU& mmu);

#endif // !KEYBOARD_HH_
