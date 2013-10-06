#include "sound.hh"

#define NB_CHANNELS 4

#define SAMPLE_RATE 44100
#define SAMPLE_COUNT 4410

#define BITS 16

#define MAX_FREQ (1 << (BITS - 1)) - 1
#define MIN_FREQ -(1 << (BITS - 1))

static void sound_callback(void* userdata, Uint8* stream, int len);

Sound::Sound(const MMU& mmu)
{
    SDL_AudioSpec spec;

    memset(&spec, 0, sizeof (SDL_AudioSpec));
    spec.channels = 2;
    spec.format = AUDIO_S16;
    spec.freq = SAMPLE_RATE;
    spec.samples = SAMPLE_COUNT;

    spec.callback = sound_callback;
    spec.userdata = (void*) &mmu;

    int dev = SDL_OpenAudio(&spec, nullptr);
    if (dev < 0) {
        logging::error("Failed to open audio device: %s\n", SDL_GetError());
    }
    SDL_PauseAudio(0); // Activate audio.
}

Sound::~Sound() {
    SDL_CloseAudio();
}

typedef void (*chandler)(const MMU*, int32_t*, int);

static void channel1(const MMU* mmu, int32_t* data, int len);
static void channel2(const MMU* mmu, int32_t* data, int len);
static void channel3(const MMU* mmu, int32_t* data, int len);
static void channel4(const MMU* mmu, int32_t* data, int len);

static void sound_callback(void* userdata, Uint8* stream, int len) {
    chandler handlers[NB_CHANNELS] = { channel1, channel2, channel3, channel4 };
    const MMU* mmu = (const MMU*) userdata;
    int16_t* stream_ = (int16_t*) stream;
    int32_t data[len];

    memset(data, 0, len);
    for (int it = 0; it < NB_CHANNELS; ++it) {
        handlers[it](mmu, data, len / 2);
    }
    for (int it = 0; it < len / 2; it += 2) {
        // Avoid saturation.
        if (MAX_FREQ <= data[it]) data[it] = MAX_FREQ;
        if (data[it] < MIN_FREQ) data[it] = MIN_FREQ;
        stream_[it] = stream_[it + 1] = data[it];
    }
}

// Channel 1: Tone & Sweep
static uint32_t count_channel1 = 0;
static void channel1(const MMU* mmu, int32_t* data, int len) {
    (void) mmu;
    float freqs[] = { 261., 293., 329., 349., 391., 440., 493., 523.};
    for (int it = 0; it < len; ++it, ++count_channel1) {
        float a = sin(2. * PI * freqs[(count_channel1 / 40000) % 8] *
                      (static_cast<float>(count_channel1) / SAMPLE_RATE));
        data[it] = (a < 0 ? MIN_FREQ : MAX_FREQ);
    }
}

// Channel 2: Tone
static void channel2(const MMU* mmu, int32_t* data, int len) {
    (void) mmu;
    (void) data;
    (void) len;
}

// Channel 3: Wave Output
static void channel3(const MMU* mmu, int32_t* data, int len) {
    (void) mmu;
    (void) data;
    (void) len;
}

// Channel 4: Noise
static void channel4(const MMU* mmu, int32_t* data, int len) {
    (void) mmu;
    (void) data;
    (void) len;
}
