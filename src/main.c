// TODO: 1. Add different wav samples and add arguments to let user decide which
// sample to play on each keypress .
// TODO: 2. Learn how to wrap it directly into package manager or something like
// aur helper.
// TODO: 3. Let user control volume irrespective of systems volume.
// TODO: 4. look for a better way to handle key events and audio parallel to
// increase efficiency and minimize system usage.

#include "./lib/audio.h"
#include <errno.h>
#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_SIZE 256

// flag to stop infinite while loop.
volatile sig_atomic_t running = 1;

// Shared queue and thread synchronization.
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

// Stores key events before being processed by the thread.
char *key_queue[QUEUE_SIZE];
int queue_head = 0, queue_tail = 0;

// Signal handler(gets called when ctrl c is pressed via signla).
void sigint_handler(int sig_num) {
  (void)sig_num; // Silence unused parameter warning.
  printf("\nCtrl+C Pressed, Exiting Gracefully\n");

  // changes flag to stop the loop.
  running = 0;
}

// Add key to the queue (enqueue)
void enqueue_key(const char *key) {
  // only one thread can modify it at a time.
  pthread_mutex_lock(&queue_mutex);

  if ((queue_tail + 1) % QUEUE_SIZE != queue_head) {
    // duplicate key into dynamic allocated memory.
    key_queue[queue_tail] = strdup(key);
    queue_tail = (queue_tail + 1) % QUEUE_SIZE;
    pthread_cond_signal(&queue_cond); // Wake up audio thread
  } else {
    // queue is full
    fprintf(stderr, "Queue full, dropping key event\n");
  }
  pthread_mutex_unlock(&queue_mutex);
}

// Remove key from the queue (dequeue)
char *dequeue_key() {
  // prevent access to both thread, let only one modify it.
  pthread_mutex_lock(&queue_mutex);

  while (queue_head == queue_tail && running) {
    pthread_cond_wait(&queue_cond, &queue_mutex);
  }

  if (!running) {
    pthread_mutex_unlock(&queue_mutex);
    return NULL;
  }

  char *key = key_queue[queue_head];
  queue_head = (queue_head + 1) % QUEUE_SIZE;

  pthread_mutex_unlock(&queue_mutex);
  return key;
}

// Audio thread function
void *audio_thread(void *arg) {
  (void)arg;
  init_audio();
  while (running) {
    char *key = dequeue_key();
    if (key) {
      // printf("Playing sound for: %s\n", key);
      play_wav("./assets/s1.wav"); // SDL2 handles concurrent sounds
      free(key);
    }
  }
  close_audio();
  return NULL;
}

// Libinput helper functions
static int open_restricted(const char *path, int flags, void *user_data) {
  (void)user_data;
  int fd = open(path, flags);
  if (fd < 0) {
    fprintf(stderr, "Failed to open %s: %s\n", path, strerror(errno));
  }
  return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) {
  (void)user_data;
  close(fd);
}

static const struct libinput_interface interface = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

int main(void) {
  signal(SIGINT, sigint_handler);

  // deploy separate thread for audio and main.
  pthread_t audio_tid;
  pthread_create(&audio_tid, NULL, audio_thread, NULL);

  // helps detect automatically if input device is plugged out.
  struct udev *udev = udev_new();
  if (!udev) {
    fprintf(stderr, "Failed to create udev context\n");
    return 1;
  }

  struct libinput *context =
      libinput_udev_create_context(&interface, NULL, udev);
  if (!context) {
    fprintf(stderr, "Failed to create libinput context\n");
    udev_unref(udev);
    return 1;
  }

  if (libinput_udev_assign_seat(context, "seat0") != 0) {
    fprintf(stderr, "Failed to assign seat\n");
    libinput_unref(context);
    udev_unref(udev);
    return 1;
  }

  printf("Monitoring keyboard events...\n");

  while (running) {
    // main event dispatchment function.
    libinput_dispatch(context);
    struct libinput_event *event;

    while ((event = libinput_get_event(context))) {
      enum libinput_event_type type = libinput_event_get_type(event);
      if (type == LIBINPUT_EVENT_KEYBOARD_KEY) {
        struct libinput_event_keyboard *kb_event =
            libinput_event_get_keyboard_event(event);
        uint32_t key = libinput_event_keyboard_get_key(kb_event);
        uint32_t state = libinput_event_keyboard_get_key_state(kb_event);

        if (state == LIBINPUT_KEY_STATE_PRESSED) {
          char key_str[15];
          // extracting raw code to name queue events.
          snprintf(key_str, sizeof(key_str), "Key-%u", key);
          enqueue_key(key_str);
        }
      }
      libinput_event_destroy(event);
    }
  }

  // clean up memory and terminate threads.
  pthread_cancel(audio_tid);
  pthread_join(audio_tid, NULL);
  close_audio();
  libinput_unref(context);
  udev_unref(udev);

  return 0;
}
