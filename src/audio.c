#include <alsa/asoundlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100
#define FREQUENCY 440
#define DURATION 0.03

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
  if (check_error(snd_pcm_hw_params_set_channels(_soundDevice, hw_params, 2))) {
    return 1;
  }
  if (check_error(snd_pcm_hw_params(_soundDevice, hw_params))) {
    return 1;
  }
  snd_pcm_hw_params_free(hw_params);
  return 0;
}

void close_audio() {
  if (_soundDevice) {
    snd_pcm_close(_soundDevice);
    _soundDevice = NULL;
  }
}
int play_beep() {
  if (!_soundDevice) {
    if (init_audio())
      return 1;
  }
  snd_pcm_drop(_soundDevice);
  snd_pcm_prepare(_soundDevice);

  // Generate sine wave buffer
  snd_pcm_uframes_t frames = SAMPLE_RATE * DURATION;
  int16_t *buffer = malloc(frames * 2 * sizeof(int16_t));
  if (!buffer) {
    fprintf(stderr, "Failed to allocate buffer\n");
    return 1;
  }

  for (snd_pcm_uframes_t i = 0; i < frames; i++) {
    int16_t sample = 32767 * sin(2.0 * M_PI * FREQUENCY * i / SAMPLE_RATE);
    buffer[i * 2] = sample;
    buffer[i * 2 + 1] = sample;
  }
  if (check_error(snd_pcm_writei(_soundDevice, buffer, frames))) {
    free(buffer);
    return 1;
  }

  free(buffer);
  return 0;
}
