# CrunchyKeys

## 🚧 STILL IN DEVELOPMENT 🚧

CrunchyKeys is a tool built in C to produce mechanical keyboard sounds on each keypress.  
Currently, it is developed only for Linux and works on both Xorg and Wayland.

---

## 📦 Installation

### Dependencies

Ensure you have ALSA, Make, and Libinput installed.  
Also, make sure your user is in the `input` group. If not, add yourself with:

```bash
sudo usermod -aG input $USER
```

### Install Required Packages

- **Debian/Ubuntu:**
  ```bash
  sudo apt install libasound2-dev libinput-dev
  ```
- **Arch Linux:**
  ```bash
  sudo pacman -S alsa-lib libinput
  ```
- **Other Distributions:**  
  Install ALSA and Libinput using your package manager.

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
