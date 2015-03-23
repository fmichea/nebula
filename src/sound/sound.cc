#include "sound.hh"

static void sound_callback(void* userdata, Uint8* stream, int len);

Sound::Sound(MMU* mmu)
    : mmu_ (mmu)
{
    // Init channels.
    for (int it = 0; it < NB_GB_CHANNELS; ++it)
        this->channels_[it] = nullptr;
    this->channels_[0] = new Channel(mmu, 1, mmu_->NR13, mmu_->NR14, {
        new Sweep(1, mmu_->NR52, mmu_->NR10),
        new nebula::sound::filters::channels::Square(),
        //new nebula::sound::filters::Timer(),
        new WaveForm(mmu_->NR11, mmu_->NR13, mmu_->NR14),
        new Length(1, mmu_->NR52, mmu_->NR11, mmu_->NR14),
        new VolumeEnvelop(mmu_->NR12),
    });
    this->channels_[1] = new Channel(mmu, 2, mmu_->NR23, mmu_->NR24, {
        //new nebula::sound::filters::Timer(),
        new nebula::sound::filters::channels::Square(),
        new WaveForm(mmu_->NR21, mmu_->NR23, mmu_->NR24),
        new Length(2, mmu_->NR52, mmu_->NR21, mmu_->NR24),
        new VolumeEnvelop(mmu_->NR22),
    });
#if 0
    this->channels_[2] = new Channel(3, mmu_->NR52, mmu_->NR33, mmu_->NR34, {
        new Length(3, mmu_->NR52, mmu_->NR31, mmu_->NR34),
    });
    this->channels_[3] = new Channel(4, mmu_->NR52, mmu_->NR33, mmu_->NR34, {
        new Length(4, mmu_->NR52, mmu_->NR41, mmu_->NR44),
    });
#endif

    // Initialize SDL.
    SDL_InitSubSystem(SDL_INIT_AUDIO);

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
    int32_t     data, count;

    // Buffer is not initialized by default.
    memset(stream, 0, _len);

    // If sound is not ON, end there.
    if (this->mmu_->NR52.sound_on.get() == 0)
        return;
    //logging::info("sound is ON!");

    for (int it = 0; it < NB_GB_CHANNELS; ++it) {
        channels[it] = new int16_t[len];
        memset(channels[it], 0, len * sizeof (int16_t));
        if (this->channels_[it] == nullptr)
            continue;
        this->channels_[it]->fill_stream(channels[it], len);
    }

    for (int chan = 0; chan < NB_CHANNELS; ++chan) {
        for (int it = chan; it < len; it += 2) {
            // Fetch the frequency computed.
            data = 0, count = 0;
            for (int it_ = 0; it_ < NB_GB_CHANNELS; ++it_) {
                if ((1 << (4 * chan + it_)) & this->mmu_->NR51.get()) {
                    if (channels[it_][it]) {
                        data += channels[it_][it];
                        count += 1;
                    }
                }
            }
            if (count != 0)
                data /= count;

            // Volume fix.
            data *= this->mmu_->NR50.volume[chan].get();
            data /= 7;

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
