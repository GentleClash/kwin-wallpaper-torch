#include "wallpapertorch.h"

#include "core/rendertarget.h"
#include "core/renderviewport.h"
#include "effect/effecthandler.h"
#include "opengl/glutils.h"

namespace KWin
{
static const QByteArray s_vertexShader = QByteArrayLiteral(R"GLSL(
#version 140

uniform mat4 modelViewProjectionMatrix;

in vec4 vertex;
in vec2 texcoord;

out vec2 texcoord0;

void main()
{
    texcoord0   = texcoord;
    gl_Position = modelViewProjectionMatrix * vertex;
}
)GLSL");

// ---------------------------------------------------------------------------
// Fully self-contained fragment shader.
//
// Y-axis note:
//   effects->cursorPos() and window->frameGeometry() are Y-down screen coords.
//   GL texcoords have Y=0 at the bottom of the texture (Y-up).
//   Conversion: screenY = windowTop + (1.0 - uv.y) * windowHeight
// ---------------------------------------------------------------------------
static const QByteArray s_fragmentShader = QByteArrayLiteral(R"GLSL(
#version 140

uniform sampler2D sampler;
uniform vec2      cursorPos;
uniform vec2      windowPos;
uniform vec2      windowSize;
uniform float     radius;
uniform float     darkness;

in  vec2 texcoord0;
out vec4 fragColor;

void main()
{
    vec4 color = texture(sampler, texcoord0);

    // Screen-pixel position of this fragment (Y-down).
    vec2 pixelPos = vec2(
        windowPos.x + texcoord0.x * windowSize.x,
        windowPos.y + (1.0 - texcoord0.y) * windowSize.y
    );

    float dist   = length(pixelPos - cursorPos);
    float shadow = smoothstep(radius * 0.3, radius, dist);

    color.rgb *= (1.0 - darkness * shadow);

    fragColor = color;
}
)GLSL");

// ---------------------------------------------------------------------------

WallpaperTorchEffect::WallpaperTorchEffect()
{
    m_shader = ShaderManager::instance()->loadShaderFromCode(
        s_vertexShader,
        s_fragmentShader
    );

    if (!m_shader || !m_shader->isValid()) {
        qWarning("WallpaperTorch: failed to compile spotlight shader");
    } else {
        qDebug("WallpaperTorch: shader compiled OK");
    }
    for (EffectWindow *w : effects->stackingOrder()) {
        if (w->isDesktop()) {
            redirect(w);          // capture it to offscreen texture
            setShader(w, m_shader.get());  // bind our shader to it
        }
    }
    connect(effects, &EffectsHandler::windowAdded, this, [this](EffectWindow *w) {
        if (w->isDesktop()) {
            redirect(w);
            setShader(w, m_shader.get());
        }
    });
    
}

WallpaperTorchEffect::~WallpaperTorchEffect() = default;

bool WallpaperTorchEffect::supported()
{
    return true;
}

bool WallpaperTorchEffect::isActive() const
{
    return true; // always on
}

void WallpaperTorchEffect::prePaintScreen(ScreenPrePaintData &data,
                                           std::chrono::milliseconds presentTime)
{
    // Force repaint every frame so the spotlight tracks the cursor live.
    effects->addRepaintFull();
    effects->prePaintScreen(data, presentTime);
}

void WallpaperTorchEffect::drawWindow(const RenderTarget &rt,
    const RenderViewport &vp, EffectWindow *window,
    int mask, const QRegion &region, WindowPaintData &data)
{
    if (window->isDesktop() && m_shader && m_shader->isValid()) {
        const QPointF cursor = effects->cursorPos();
        const QRectF  geo    = window->frameGeometry();

        // Bind shader, set uniforms (they persist in the GL program object),
        // then unbind — OffscreenEffect will rebind the same shader to draw
        ShaderManager::instance()->pushShader(m_shader.get());
        m_shader->setUniform("cursorPos",  QVector2D(float(cursor.x()), float(cursor.y())));
        m_shader->setUniform("windowPos",  QVector2D(float(geo.x()),    float(geo.y())));
        m_shader->setUniform("windowSize", QVector2D(float(geo.width()), float(geo.height())));
        m_shader->setUniform("radius",     RADIUS);
        m_shader->setUniform("darkness",   DARKNESS);
        ShaderManager::instance()->popShader();
    }
    // Parent draws the offscreen texture using our shader with uniforms now set
    OffscreenEffect::drawWindow(rt, vp, window, mask, region, data);
}
} // namespace KWin

#include "moc_wallpapertorch.cpp"