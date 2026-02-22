# kwin-wallpaper-torch

A KWin desktop effect for KDE Plasma 6 that darkens your wallpaper 
everywhere except a soft radial spotlight around your cursor.

Works through transparent windows, the lit/dark wallpaper 
shows through their transparency naturally.

<video src="https://github.com/user-attachments/assets/a67ec57f-6e4a-49f5-bb10-35913923c7cb" autoplay loop muted style="max-width: 100%;">
</video>

## Requirements

- Arch Linux (or similar) with KDE Plasma 6 running **kwin_x11**
- Packages: `kwin`, `kwin-x11`, `cmake`, `extra-cmake-modules`

> **Note:** This was built and tested specifically on Arch Linux with 
> KWin 6.5.5 under an X11 session. Wayland is untested. 
> Other distros may need path adjustments.

## Build & Install

```bash
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build -j$(nproc)
sudo cmake --install build
kwin_x11 --replace &
```

Then enable in System Settings -> Desktop Effects -> WallpaperTorch

## Configuration

Edit these two constants in src/wallpapertorch.h and rebuild:

```cpp
static constexpr float RADIUS   = 250.0f; // spotlight radius in pixels
static constexpr float DARKNESS = 0.85f;  // 0.0 = off, 1.0 = pitch black
```


