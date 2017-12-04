#pragma once

#include "Core/CoreTypes.h"
#include <string>

namespace sf { class RenderWindow; }
class CInputSystem;

enum EWindowStyle {
    e_windowStyleNone = 0,
    e_windowStyleTitlebar = 1 << 0,
    e_windowStyleResize = 1 << 1,
    e_windowStyleClose = 1 << 2,
    e_windowStyleFullscreen = 1 << 3
};

struct SWindowContext {
    uint32_t m_width{};
    uint32_t m_height{};
    uint32_t m_bitsPerPixel{32};
    uint32_t m_style{e_windowStyleTitlebar | e_windowStyleResize | e_windowStyleClose };
    std::string m_title{"Unnamed"};
};

class CWindow {
public:
    CWindow ();
    ~CWindow ();

    void Create (const SWindowContext& context);

    bool IsOpen () const;
    void Close ();
    void ProcessInput (CInputSystem& input);

    void Clear ();
    void Display ();

private:
    sf::RenderWindow* m_window{nullptr};
};