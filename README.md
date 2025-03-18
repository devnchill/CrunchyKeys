# CrunchyKeys

## 🚧 STILL IN DEVELOPMENT 🚧

CrunchyKeys is a tool built in C to produce mechanical keyboard sounds on each keypress.  
Currently, it is developed only for Linux and works on both Xorg and Wayland.

It now uses **SDL2** for audio playback instead of ALSA.

---

## 📦 Installation

### Dependencies

Ensure you have **SDL2**, **Make**, and **Libinput** installed.
Also, make sure your user is in the `input` group. If not, add yourself with:

```bash
sudo usermod -aG input $USER
```

### Install Required Packages

- **Debian/Ubuntu:**
  ```bash
  sudo apt install libsdl2-dev libinput-dev
  ```
- **Arch Linux:**
  ```bash
  sudo pacman -S sdl2 libinput
  ```
- **Other Distributions:**  
  Install SDL2 and Libinput using your package manager.

---

## ⚙️ Setup

Clone the repository, build the project using `make`, and run the executable:

```bash
git clone https://github.com/devnchill/CrunchyKeys
cd CrunchyKeys
make
./build/crunchykeys
```

## 🛑 Stopping the Program

To exit CrunchyKeys, press:

```bash
CTRL + C
```
