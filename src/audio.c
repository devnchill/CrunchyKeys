#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#define AUDIO_FREQUENCY 44100
#define AUDIO_FORMAT MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS 2
#define AUDIO_CHUNKSIZE 1024

static Mix_Chunk *sound = NULL;

int init_audio() {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  if (Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_FORMAT, AUDIO_CHANNELS,
                    AUDIO_CHUNKSIZE) < 0) {
    fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
    return 1;
  }

  // get chunks to be played.
  sound = Mix_LoadWAV("./assets/s1.wav");
  if (!sound) {
    fprintf(stderr, "Failed to load WAV file: %s\n", Mix_GetError());
    return 1;
  }
  return 0;
}

void play_wav(const char *filename) {
  (void)filename;
  //-1 is the channel in which the chunk gets played.
  Mix_PlayChannel(-1, sound, 0);
}

void close_audio() {
  if (sound) {
    // if music is already playing , would stop thi.
    Mix_FreeChunk(sound);
    sound = NULL;
  }
  Mix_CloseAudio();
  SDL_Quit();
}
