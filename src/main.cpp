#include "wallpapertorch.h"

namespace KWin
{

KWIN_EFFECT_FACTORY_SUPPORTED(WallpaperTorchEffect,
                              "metadata.json",
                              return WallpaperTorchEffect::supported();)

} // namespace KWin

#include "main.moc"
