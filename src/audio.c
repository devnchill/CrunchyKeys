#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100

static snd_pcm_t *_soundDevice = NULL;

// Function to check and handle ALSA errors
int check_error(int err) {
  if (err < 0) {
    fprintf(stderr, "Error: %s\n", snd_strerror(err));
    return 1;
  }
  return 0;
}

int init_audio() {
  if (_soundDevice) {
    return 0;
  }
  snd_pcm_hw_params_t *hw_params;
  int err;
  snd_pcm_uframes_t buffer_size = 4096;
  snd_pcm_uframes_t period_size = 1024;

  if (check_error(
          snd_pcm_open(&_soundDevice, "default", SND_PCM_STREAM_PLAYBACK, 0))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params_malloc(&hw_params))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params_any(_soundDevice, hw_params))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params_set_access(
          _soundDevice, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params_set_format(_soundDevice, hw_params,
                                               SND_PCM_FORMAT_S16_LE))) {
    return 1;
  }

  unsigned int sample_rate = SAMPLE_RATE;
  int dir = 0;
  if (check_error(snd_pcm_hw_params_set_rate_near(_soundDevice, hw_params,
                                                  &sample_rate, &dir))) {
    return 1;
  }

  if (check_error(snd_pcm_hw_params_set_buffer_size_near(
          _soundDevice, hw_params, &buffer_size))) {
    return 1;
  }

  if (check_error(snd_pcm_hw_params_set_period_size_near(
          _soundDevice, hw_params, &period_size, 0))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params_set_channels(_soundDevice, hw_params, 2))) {
    return 1;
  }

  if (check_error(snd_pcm_hw_params(_soundDevice, hw_params))) {
    return 1;
  }
  snd_pcm_hw_params_free(hw_params);
  if (check_error(snd_pcm_prepare(_soundDevice))) {
    return 1;
  }
  return 0;
}

void close_audio() {
  if (_soundDevice) {
    snd_pcm_close(_soundDevice);
    _soundDevice = NULL;
  }
}
int play_wav(const char *filename) {
  if (!_soundDevice) {
    if (init_audio())
      return 1;
  }
  snd_pcm_drop(_soundDevice);
  snd_pcm_prepare(_soundDevice);
  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open WAV file: %s\n", filename);
    return 1;
  }

  fseek(file, 44, SEEK_SET);
  int16_t buffer[1024];
  size_t read_samples;
  while ((read_samples = fread(buffer, sizeof(int16_t), 1024, file)) > 0) {
    int err;
    if ((err = snd_pcm_writei(_soundDevice, buffer, read_samples / 2)) < 0) {
      if (err == -EPIPE) { // Buffer underrun (xruns)
        fprintf(stderr, "Buffer underrun detected! Recovering...\n");
        snd_pcm_prepare(_soundDevice); // Recover
      } else {
        fprintf(stderr, "ALSA write error: %s\n", snd_strerror(err));
        fclose(file);
        return 1;
      }
    }
  }
  fclose(file);
  return 0;
}
