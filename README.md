# CrunchyKeys

## STILL IN DEVELOPMENT.

A tool built in C to produce mechanical keyboard sounds on each keypress.At the moment , it's developed only for linux (works on both XOrg and Wayland).

### INSTALLATION

**DEPENDENCIES** - Install ALSA, Make and Libinput if you don't have them.

- **Debian/Ubuntu:** `sudo apt install libasound2-dev libinput-dev`
- **Arch Linux:** `sudo pacman -S alsa-lib libinput`
- **For other distributions, install ALSA and Libinput using your package manager.**

### SETUP

- Clone this repo. Use `make` to build and run the executable. This will compile the files into `./build/`.

```bash
git clone https://github.com/devnchill/CrunchyKeys
cd CrunchyKeys
make
./build/crunchykeys
```
