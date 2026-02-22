#pragma once
#include "effect/offscreeneffect.h"
#include <memory>

namespace KWin {
class GLShader;

class WallpaperTorchEffect : public OffscreenEffect
{
    Q_OBJECT
public:
    WallpaperTorchEffect();
    ~WallpaperTorchEffect() override;
    static bool supported();
    bool isActive() const override;
    void prePaintScreen(ScreenPrePaintData &data,
                        std::chrono::milliseconds presentTime) override;
    void drawWindow(const RenderTarget &renderTarget,
                    const RenderViewport &viewport,
                    EffectWindow *window, int mask,
                    const QRegion &region, WindowPaintData &data) override;
private:
    std::unique_ptr<GLShader> m_shader;
    static constexpr float RADIUS   = 250.0f;
    static constexpr float DARKNESS = 0.95f;
};
} // namespace KWin
