#include "./include/keymap.h"
#include "include/audio.h"
#include <errno.h>
#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int open_restricted(const char *path, int flags, void *user_data) {
  int fd = open(path, flags);
  if (fd < 0)
    fprintf(stderr, "Failed to open %s: %s\n", path, strerror(errno));
  return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data) { close(fd); }

static const struct libinput_interface interface = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

int main(void) {
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

  while (1) {
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
          play_wav("./assets/s1.wav");
          char *key_name = get_key_name(key);
          printf("%s is the key name\n", key_name);
        }
      }
      libinput_event_destroy(event);
    }
  }

  close_audio();
  libinput_unref(context);
  udev_unref(udev);
  return 0;
}
