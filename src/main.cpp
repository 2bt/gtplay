#include <cstdio>
#include <cstring>
#include <algorithm>
#include <SDL2/SDL.h>
#include "gplayer.hpp"
#include "resid/sid.h"


enum {
    MIXRATE           = 44100,
    PALCLOCKRATE      = 985248,
    SAMPLES_PER_FRAME = MIXRATE / 50,
    BUFFER_SIZE       = 1024 * 2
};


class SidEngine {
public:
    SidEngine() {
        m_sid.reset();
        m_sid.set_chip_model(MOS8580);
        m_sid.set_sampling_parameters(PALCLOCKRATE, SAMPLE_RESAMPLE_INTERPOLATE, MIXRATE);
    }
    void set_reg(int r, uint8_t value) {
        m_sid.write(r, value);
    }
    void mix(int16_t* buffer, int length) {
        int c = 999999999;
        m_sid.clock(c, buffer, length);
    }
private:
    SID     m_sid;
    uint8_t m_regs[25] = {};
};


gt::Song   song;
gt::Player player(song);
SidEngine  sid_engine;


void audio_callback(void* u, Uint8* stream, int bytes) {
    int16_t* buffer = (int16_t*) stream;
    int      length = bytes / sizeof(int16_t);
    static int sample = 0;
    while (length > 0) {
        if (sample == 0) {
            player.play_routine();
            for (int i = 0; i < 25; ++i) sid_engine.set_reg(i, player.regs[i]);
        }
        int l = std::min(SAMPLES_PER_FRAME - sample, length);
        sample += l;
        if (sample == SAMPLES_PER_FRAME) sample = 0;
        length -= l;
        sid_engine.mix(buffer, l);
        buffer += l;
    }
}



int main(int argc, char** argv) {

    if (argc != 2) {
        printf("usage: %s sng-file\n", argv[0]);
        return 1;
    }

    if (!song.load(argv[1])) {
        printf("ERROR\n");
        return 1;
    }

    player.init_song(0, gt::Player::PLAY_BEGINNING);

    SDL_AudioSpec spec = { MIXRATE, AUDIO_S16, 1, 0, BUFFER_SIZE, 0, 0, &audio_callback };
    SDL_OpenAudio(&spec, nullptr);
    SDL_PauseAudio(0);
    getchar();

    return 0;
}
