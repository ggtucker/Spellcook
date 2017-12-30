#pragma once

#include "Core/CoreTypes.h"
#include "InputEvent.h"
#include <queue>
#include <string>

struct GLFWwindow;

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
    CWindow () = default;
    ~CWindow ();

    bool Create (const SWindowContext& context);

    bool IsOpen () const;
    void Close ();
    void SetActive (bool active);

    bool PollInput (CInputEvent& inputEvent);
    void PushInput (const CInputEvent& inputEvent);

    void Clear ();
    void SwapBuffers ();

    bool IsKeyRepeatEnabled () const { return m_keyRepeatEnabled; }
    void SetKeyRepeatEnabled (bool enabled) { m_keyRepeatEnabled = enabled; }

private:
    GLFWwindow* m_window{nullptr};
    std::queue<CInputEvent> m_inputQueue{};
    bool m_keyRepeatEnabled{};
};

extern CWindow g_window;