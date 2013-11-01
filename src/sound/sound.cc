#include "sound.hh"

static void sound_callback(void* userdata, Uint8* stream, int len);

Sound::Sound(const MMU& mmu)
    : mmu_ (mmu)
{
    // Init channels.
    for (int it = 0; it < NB_GB_CHANNELS; ++it)
        this->channels_[it] = nullptr;
    this->channels_[0] = new Channel1(this->mmu_);

    // Initialize SDL.

    memset(&this->spec_, 0, sizeof (SDL_AudioSpec));
    this->spec_.channels = NB_CHANNELS;
    this->spec_.format = AUDIO_S16SYS;
    this->spec_.freq = SAMPLE_RATE;
    this->spec_.samples = SAMPLE_COUNT;

    this->spec_.callback = sound_callback;
    this->spec_.userdata = (void*) this;

    int dev = SDL_OpenAudio(&this->spec_, nullptr);
    if (dev < 0) {
        logging::error("Failed to open audio device: %s\n", SDL_GetError());
    }
    SDL_PauseAudio(0); // Activate audio.
}

void Sound::fill_stream(Uint8* stream, int _len) {
    int         len = _len / sizeof (Sint16);
    int16_t*    channels[NB_GB_CHANNELS];
    int16_t*    stream_ = new int16_t[len];
    int32_t     data;

    // Buffer is not initialized by default.
    memset(stream, this->spec_.silence, _len);

    // If sound is not ON, end there.
    if ((this->mmu_.NR52.get() & (1 << 7)) == 0)
        return;

    for (int it = 0; it < NB_GB_CHANNELS; ++it) {
        channels[it] = new int16_t[len];
        memset(channels[it], 0, len * sizeof (int16_t));
        if (this->channels_[it] == nullptr)
            continue;
//        if ((this->mmu_.NR52.get() & (1 << it)) == 0)
//            continue;
        this->channels_[it]->fill_stream(channels[it], len);
    }

    for (int chan = 0; chan < NB_CHANNELS; ++chan) {
        for (int it = 0; it < len; it++) {
            // Fetch the frequency computed.
            data = 0;
            for (int it_ = 0; it_ < NB_GB_CHANNELS; ++it_) {
                if ((1 << (4 * chan + it_)) & this->mmu_.NR51.get())
                    data += channels[it_][it];
            }

            // Avoid saturation.
            if (MAX_FREQ < data) data = MAX_FREQ;
            if (data < MIN_FREQ) data = MIN_FREQ;

            // Add sound to outputs.
            stream_[it] = (Sint16) data;
        }
    }

    //SDL_MixAudioFormat(stream, (Uint8*) stream_, AUDIO_S16SYS, len, 10);
    memcpy(stream, stream_, _len);

    for (int it = 0; it < NB_GB_CHANNELS; ++it)
        delete[] channels[it];
    delete[] stream_;
}

Sound::~Sound() {
    // Delete channels from memory.
    for (int it = 0; it < NB_GB_CHANNELS; ++it) {
        delete this->channels_[it];
    }

    // Close SDL sound system.
    SDL_CloseAudio();
}

static void sound_callback(void* userdata, Uint8* _stream, int len) {
    Sound* sound = static_cast<Sound*>(userdata);

    if (sound == NULL)
        return;
    sound->fill_stream(_stream, len);
}
