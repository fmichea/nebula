#include "keyboard.hh"

static s_keybinding keybindings[NB_KEYBINDINGS] = {
    // Direction Keys.
    { .key = SDLK_DOWN, .bit = 3, .request = 4 },
    { .key = SDLK_UP, .bit = 2, .request = 4 },
    { .key = SDLK_LEFT, .bit = 1, .request = 4 },
    { .key = SDLK_RIGHT, .bit = 0, .request = 4 },

    // Button Keys.
    { .key = SDLK_SPACE, .bit = 3, .request = 5 }, // Start
    { .key = SDLK_RETURN, .bit = 2, .request = 5 }, // Select
    { .key = SDLK_q, .bit = 1, .request = 5 }, // A
    { .key = SDLK_a, .bit = 0, .request = 5 }, // B
};

void keyboard_cycle(MMU& mmu)
{
    SDL_Event   event;
    uint8_t     joyp;
    bool        joyp_changed = false;

    joyp = mmu.read<uint8_t>(0xFF00);
    while (SDL_PollEvent(&event))
    {
        // FIXME: SDL_QUIT to stop processor and fix STOP opcode also.
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            for (int it = 0; it < NB_KEYBINDINGS; it++)
            {
                if (((joyp >> keybindings[it].request) & 0x1) &&
                    event.key.keysym.sym == keybindings[it].key)
                {
                    BitProxy proxy(&joyp, keybindings[it].bit, 0x1);
                    proxy.set(event.type == SDL_KEYDOWN ? 0 : 1);

                    BitProxy req(&joyp, keybindings[it].request, 0x1);
                    req.set(0);

                    joyp_changed = true;
                }
            }
        }
    }
    mmu.write<uint8_t>(0xFF00, joyp);

    // Keyboard interrupt requested when key pressed.
    if (joyp_changed)
        mmu.IF.set(mmu.IF.get() | 0x10);
}
