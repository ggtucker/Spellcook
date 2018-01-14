#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/CoreDebug.h"

CWindow g_window; // define global window

DefineLogStatic_(Window, ELogCategory::Render, "Window.txt", 0);

static NKeyboard::EKey GlfwKeyCodeToKeyboardKey (int key);
static int KeyboardKeyToGlfwKeyCode (NKeyboard::EKey key);
static NMouse::EButton GlfwMouseCodeToMouseButton (int button);
static int MouseButtonToGlfwMouseCode (NMouse::EButton button);

static void GlfwFramebufferSizeCallback (GLFWwindow* window, int width, int height);
static void GlfwKeyCallback (GLFWwindow* window, int key, int scanCode, int action, int mode);
static void GlfwMouseButtonCallback (GLFWwindow* window, int button, int action, int mods);
static void GlfwMouseMoveCallback (GLFWwindow* window, double xpos, double ypos);
static void GlfwMouseEnterCallback (GLFWwindow* window, int entered);
static void GlfwMouseScrollCallback (GLFWwindow* window, double xoffset, double yoffset);
static void GlfwResizeCallback (GLFWwindow* window, int width, int height);
static void GlfwCloseCallback (GLFWwindow* window);

CWindow::~CWindow () {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool CWindow::Create (const SWindowContext& context) {
    if (!glfwInit()) {
        LogFatal_(Window, "Failed to initialize glfw");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(
        context.m_width,
        context.m_height,
        context.m_title.c_str(),
        nullptr,
        nullptr
    );
    if (m_window == nullptr) {
        LogFatal_(Window, "Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LogFatal_(Window, "Failed to initialize GLAD");
        glfwTerminate();
        return false;
    }
    glViewport(0, 0, context.m_width, context.m_height);
    glfwSetFramebufferSizeCallback(m_window, GlfwFramebufferSizeCallback);

    glfwSetKeyCallback(m_window, GlfwKeyCallback);
    glfwSetMouseButtonCallback(m_window, GlfwMouseButtonCallback);
    glfwSetCursorPosCallback(m_window, GlfwMouseMoveCallback);
    glfwSetCursorEnterCallback(m_window, GlfwMouseEnterCallback);
    glfwSetScrollCallback(m_window, GlfwMouseScrollCallback);
    glfwSetWindowSizeCallback(m_window, GlfwResizeCallback);
    glfwSetWindowCloseCallback(m_window, GlfwCloseCallback);

    return true;
}

bool CWindow::IsOpen () const {
    return !glfwWindowShouldClose(m_window);
}

void CWindow::Close () {
    glfwSetWindowShouldClose(m_window, true);
}

void CWindow::SetActive (bool active) {
    glfwMakeContextCurrent(active ? m_window : nullptr);
}

bool CWindow::PollInput (CInputEvent& inputEvent) {
    if (m_inputQueue.empty()) {
        glfwPollEvents();
    }
    if (!m_inputQueue.empty()) {
        inputEvent = m_inputQueue.front();
        m_inputQueue.pop();
        return true;
    }
    return false;
}

void CWindow::PushInput (const CInputEvent& inputEvent) {
    m_inputQueue.push(inputEvent);
}

bool CWindow::IsKeyDown (NKeyboard::EKey key) const {
    return glfwGetKey(m_window, KeyboardKeyToGlfwKeyCode(key)) == GLFW_PRESS;
}

bool CWindow::IsMouseButtonDown (NMouse::EButton button) const {
    return glfwGetMouseButton(m_window, MouseButtonToGlfwMouseCode(button)) == GLFW_PRESS;
}

void CWindow::GetMousePosition (int32_t& mouseX, int32_t& mouseY) const {
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    mouseX = static_cast<int32_t>(x);
    mouseY = static_cast<int32_t>(y);
}

void CWindow::Clear () {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void CWindow::SwapBuffers () {
    glfwSwapBuffers(m_window);
}

bool CWindow::IsCursorDisabled () const {
    return glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void CWindow::SetCursorDisabled (bool disabled) {
    glfwSetInputMode(m_window, GLFW_CURSOR, disabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

static void GlfwFramebufferSizeCallback (GLFWwindow* glfwWindow, int width, int height) {
    glViewport(0, 0, width, height);
}

static void GlfwKeyCallback (GLFWwindow* glfwWindow, int key, int scanCode, int action, int mode) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    if (!window->IsKeyRepeatEnabled() && action == GLFW_REPEAT) {
        return;
    }
    CInputEvent event;
    event.m_key.m_key = GlfwKeyCodeToKeyboardKey(key);
    event.m_key.m_shift = mode & GLFW_MOD_SHIFT;
    event.m_key.m_control = mode & GLFW_MOD_CONTROL;
    event.m_key.m_alt = mode & GLFW_MOD_ALT;
    event.m_key.m_system = mode & GLFW_MOD_SUPER;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        event.m_type = EInputEvent::KeyPressed;
    }
    else if (action == GLFW_RELEASE) {
        event.m_type = EInputEvent::KeyReleased;
    }
    window->PushInput(event);
}

static void GlfwMouseButtonCallback (GLFWwindow* glfwWindow, int button, int action, int mods) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_mouseButton.m_button = GlfwMouseCodeToMouseButton(button);
    double x, y;
    glfwGetCursorPos(glfwWindow, &x, &y);
    event.m_mouseButton.m_x = static_cast<int32_t>(x);
    event.m_mouseButton.m_y = static_cast<int32_t>(y);
    if (action == GLFW_PRESS) {
        event.m_type = EInputEvent::MousePressed;
    }
    else if (action == GLFW_RELEASE) {
        event.m_type = EInputEvent::MouseReleased;
    }
    window->PushInput(event);
}

static void GlfwMouseMoveCallback (GLFWwindow* glfwWindow, double xpos, double ypos) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_mouseMove.m_x = static_cast<int32_t>(xpos);
    event.m_mouseMove.m_y = static_cast<int32_t>(ypos);
    event.m_type = EInputEvent::MouseMoved;
    window->PushInput(event);
}

static void GlfwMouseEnterCallback (GLFWwindow* glfwWindow, int entered) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    if (entered) {
        event.m_type = EInputEvent::MouseEntered;
    }
    else {
        event.m_type = EInputEvent::MouseLeft;
    }
    window->PushInput(event);
}

static void GlfwMouseScrollCallback (GLFWwindow* glfwWindow, double xoffset, double yoffset) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_mouseScroll.m_x = static_cast<int32_t>(xoffset);
    event.m_mouseScroll.m_y = static_cast<int32_t>(yoffset);
    event.m_type = EInputEvent::MouseScrolled;
    window->PushInput(event);
}

static void GlfwResizeCallback (GLFWwindow* glfwWindow, int width, int height) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_resize.m_width = width;
    event.m_resize.m_height = height;
    event.m_type = EInputEvent::Resized;
    window->PushInput(event);
}

static void GlfwCloseCallback (GLFWwindow* glfwWindow) {
    if (glfwWindowShouldClose(glfwWindow)) {
        glfwSetWindowShouldClose(glfwWindow, false);
        CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
        CInputEvent event;
        event.m_type = EInputEvent::Closed;
        window->PushInput(event);
    }
}

static NKeyboard::EKey GlfwKeyCodeToKeyboardKey (int key) {
    switch (key) {
        case GLFW_KEY_A:             return NKeyboard::A;
        case GLFW_KEY_B:             return NKeyboard::B;
        case GLFW_KEY_C:             return NKeyboard::C;
        case GLFW_KEY_D:             return NKeyboard::D;
        case GLFW_KEY_E:             return NKeyboard::E;
        case GLFW_KEY_F:             return NKeyboard::F;
        case GLFW_KEY_G:             return NKeyboard::G;
        case GLFW_KEY_H:             return NKeyboard::H;
        case GLFW_KEY_I:             return NKeyboard::I;
        case GLFW_KEY_J:             return NKeyboard::J;
        case GLFW_KEY_K:             return NKeyboard::K;
        case GLFW_KEY_L:             return NKeyboard::L;
        case GLFW_KEY_M:             return NKeyboard::M;
        case GLFW_KEY_N:             return NKeyboard::N;
        case GLFW_KEY_O:             return NKeyboard::O;
        case GLFW_KEY_P:             return NKeyboard::P;
        case GLFW_KEY_Q:             return NKeyboard::Q;
        case GLFW_KEY_R:             return NKeyboard::R;
        case GLFW_KEY_S:             return NKeyboard::S;
        case GLFW_KEY_T:             return NKeyboard::T;
        case GLFW_KEY_U:             return NKeyboard::U;
        case GLFW_KEY_V:             return NKeyboard::V;
        case GLFW_KEY_W:             return NKeyboard::W;
        case GLFW_KEY_X:             return NKeyboard::X;
        case GLFW_KEY_Y:             return NKeyboard::Y;
        case GLFW_KEY_Z:             return NKeyboard::Z;
        case GLFW_KEY_0:             return NKeyboard::Num0;
        case GLFW_KEY_1:             return NKeyboard::Num1;
        case GLFW_KEY_2:             return NKeyboard::Num2;
        case GLFW_KEY_3:             return NKeyboard::Num3;
        case GLFW_KEY_4:             return NKeyboard::Num4;
        case GLFW_KEY_5:             return NKeyboard::Num5;
        case GLFW_KEY_6:             return NKeyboard::Num6;
        case GLFW_KEY_7:             return NKeyboard::Num7;
        case GLFW_KEY_8:             return NKeyboard::Num8;
        case GLFW_KEY_9:             return NKeyboard::Num9;
        case GLFW_KEY_ESCAPE:        return NKeyboard::Escape;
        case GLFW_KEY_LEFT_CONTROL:  return NKeyboard::LControl;
        case GLFW_KEY_LEFT_SHIFT:    return NKeyboard::LShift;
        case GLFW_KEY_LEFT_ALT:      return NKeyboard::LAlt;
        case GLFW_KEY_LEFT_SUPER:    return NKeyboard::LSystem;
        case GLFW_KEY_RIGHT_CONTROL: return NKeyboard::RControl;
        case GLFW_KEY_RIGHT_SHIFT:   return NKeyboard::RShift;
        case GLFW_KEY_RIGHT_ALT:     return NKeyboard::RAlt;
        case GLFW_KEY_RIGHT_SUPER:   return NKeyboard::RSystem;
        case GLFW_KEY_MENU:          return NKeyboard::Menu;
        case GLFW_KEY_LEFT_BRACKET:  return NKeyboard::LBracket;
        case GLFW_KEY_RIGHT_BRACKET: return NKeyboard::RBracket;
        case GLFW_KEY_SEMICOLON:     return NKeyboard::Semicolon;
        case GLFW_KEY_COMMA:         return NKeyboard::Comma;
        case GLFW_KEY_PERIOD:        return NKeyboard::Period;
        case GLFW_KEY_APOSTROPHE:    return NKeyboard::Apostrophe;
        case GLFW_KEY_SLASH:         return NKeyboard::Slash;
        case GLFW_KEY_BACKSLASH:     return NKeyboard::Backslash;
        case GLFW_KEY_GRAVE_ACCENT:  return NKeyboard::Grave;
        case GLFW_KEY_EQUAL:         return NKeyboard::Equal;
        case GLFW_KEY_MINUS:         return NKeyboard::Dash;
        case GLFW_KEY_SPACE:         return NKeyboard::Space;
        case GLFW_KEY_ENTER:         return NKeyboard::Enter;
        case GLFW_KEY_BACKSPACE:     return NKeyboard::Backspace;
        case GLFW_KEY_TAB:           return NKeyboard::Tab;
        case GLFW_KEY_PAGE_UP:       return NKeyboard::PageUp;
        case GLFW_KEY_PAGE_DOWN:     return NKeyboard::PageDown;
        case GLFW_KEY_END:           return NKeyboard::End;
        case GLFW_KEY_HOME:          return NKeyboard::Home;
        case GLFW_KEY_INSERT:        return NKeyboard::Insert;
        case GLFW_KEY_DELETE:        return NKeyboard::Delete;
        case GLFW_KEY_LEFT:          return NKeyboard::Left;
        case GLFW_KEY_RIGHT:         return NKeyboard::Right;
        case GLFW_KEY_UP:            return NKeyboard::Up;
        case GLFW_KEY_DOWN:          return NKeyboard::Down;
        case GLFW_KEY_NUM_LOCK:      return NKeyboard::NumpadLock;
        case GLFW_KEY_KP_ADD:        return NKeyboard::NumpadAdd;
        case GLFW_KEY_KP_SUBTRACT:   return NKeyboard::NumpadSubtract;
        case GLFW_KEY_KP_MULTIPLY:   return NKeyboard::NumpadMultiply;
        case GLFW_KEY_KP_DIVIDE:     return NKeyboard::NumpadDivide;
        case GLFW_KEY_KP_DECIMAL:    return NKeyboard::NumpadDecimal;
        case GLFW_KEY_KP_ENTER:      return NKeyboard::NumpadEnter;
        case GLFW_KEY_KP_EQUAL:      return NKeyboard::NumpadEqual;
        case GLFW_KEY_KP_0:          return NKeyboard::Numpad0;
        case GLFW_KEY_KP_1:          return NKeyboard::Numpad1;
        case GLFW_KEY_KP_2:          return NKeyboard::Numpad2;
        case GLFW_KEY_KP_3:          return NKeyboard::Numpad3;
        case GLFW_KEY_KP_4:          return NKeyboard::Numpad4;
        case GLFW_KEY_KP_5:          return NKeyboard::Numpad5;
        case GLFW_KEY_KP_6:          return NKeyboard::Numpad6;
        case GLFW_KEY_KP_7:          return NKeyboard::Numpad7;
        case GLFW_KEY_KP_8:          return NKeyboard::Numpad8;
        case GLFW_KEY_KP_9:          return NKeyboard::Numpad9;
        case GLFW_KEY_PRINT_SCREEN:  return NKeyboard::PrintScreen;
        case GLFW_KEY_CAPS_LOCK:     return NKeyboard::CapsLock;
        case GLFW_KEY_SCROLL_LOCK:   return NKeyboard::ScrollLock;
        case GLFW_KEY_PAUSE:         return NKeyboard::Pause;
        case GLFW_KEY_F1:            return NKeyboard::F1;
        case GLFW_KEY_F2:            return NKeyboard::F2;
        case GLFW_KEY_F3:            return NKeyboard::F3;
        case GLFW_KEY_F4:            return NKeyboard::F4;
        case GLFW_KEY_F5:            return NKeyboard::F5;
        case GLFW_KEY_F6:            return NKeyboard::F6;
        case GLFW_KEY_F7:            return NKeyboard::F7;
        case GLFW_KEY_F8:            return NKeyboard::F8;
        case GLFW_KEY_F9:            return NKeyboard::F9;
        case GLFW_KEY_F10:           return NKeyboard::F10;
        case GLFW_KEY_F11:           return NKeyboard::F11;
        case GLFW_KEY_F12:           return NKeyboard::F12;
        case GLFW_KEY_F13:           return NKeyboard::F13;
        case GLFW_KEY_F14:           return NKeyboard::F14;
        case GLFW_KEY_F15:           return NKeyboard::F15;
        case GLFW_KEY_F16:           return NKeyboard::F16;
        case GLFW_KEY_F17:           return NKeyboard::F17;
        case GLFW_KEY_F18:           return NKeyboard::F18;
        case GLFW_KEY_F19:           return NKeyboard::F19;
        case GLFW_KEY_F20:           return NKeyboard::F20;
        case GLFW_KEY_F21:           return NKeyboard::F21;
        case GLFW_KEY_F22:           return NKeyboard::F22;
        case GLFW_KEY_F23:           return NKeyboard::F23;
        case GLFW_KEY_F24:           return NKeyboard::F24;
        case GLFW_KEY_F25:           return NKeyboard::F25;
    }
    return NKeyboard::Unknown;
}

static int KeyboardKeyToGlfwKeyCode (NKeyboard::EKey key) {
    switch (key) {
        case NKeyboard::A:              return GLFW_KEY_A;
        case NKeyboard::B:              return GLFW_KEY_B;
        case NKeyboard::C:              return GLFW_KEY_C;
        case NKeyboard::D:              return GLFW_KEY_D;
        case NKeyboard::E:              return GLFW_KEY_E;
        case NKeyboard::F:              return GLFW_KEY_F;
        case NKeyboard::G:              return GLFW_KEY_G;
        case NKeyboard::H:              return GLFW_KEY_H;
        case NKeyboard::I:              return GLFW_KEY_I;
        case NKeyboard::J:              return GLFW_KEY_J;
        case NKeyboard::K:              return GLFW_KEY_K;
        case NKeyboard::L:              return GLFW_KEY_L;
        case NKeyboard::M:              return GLFW_KEY_M;
        case NKeyboard::N:              return GLFW_KEY_N;
        case NKeyboard::O:              return GLFW_KEY_O;
        case NKeyboard::P:              return GLFW_KEY_P;
        case NKeyboard::Q:              return GLFW_KEY_Q;
        case NKeyboard::R:              return GLFW_KEY_R;
        case NKeyboard::S:              return GLFW_KEY_S;
        case NKeyboard::T:              return GLFW_KEY_T;
        case NKeyboard::U:              return GLFW_KEY_U;
        case NKeyboard::V:              return GLFW_KEY_V;
        case NKeyboard::W:              return GLFW_KEY_W;
        case NKeyboard::X:              return GLFW_KEY_X;
        case NKeyboard::Y:              return GLFW_KEY_Y;
        case NKeyboard::Z:              return GLFW_KEY_Z;
        case NKeyboard::Num0:           return GLFW_KEY_0;
        case NKeyboard::Num1:           return GLFW_KEY_1;
        case NKeyboard::Num2:           return GLFW_KEY_2;
        case NKeyboard::Num3:           return GLFW_KEY_3;
        case NKeyboard::Num4:           return GLFW_KEY_4;
        case NKeyboard::Num5:           return GLFW_KEY_5;
        case NKeyboard::Num6:           return GLFW_KEY_6;
        case NKeyboard::Num7:           return GLFW_KEY_7;
        case NKeyboard::Num8:           return GLFW_KEY_8;
        case NKeyboard::Num9:           return GLFW_KEY_9;
        case NKeyboard::Escape:         return GLFW_KEY_ESCAPE;
        case NKeyboard::LControl:       return GLFW_KEY_LEFT_CONTROL;
        case NKeyboard::LShift:         return GLFW_KEY_LEFT_SHIFT;
        case NKeyboard::LAlt:           return GLFW_KEY_LEFT_ALT;
        case NKeyboard::LSystem:        return GLFW_KEY_LEFT_SUPER;
        case NKeyboard::RControl:       return GLFW_KEY_RIGHT_CONTROL;
        case NKeyboard::RShift:         return GLFW_KEY_RIGHT_SHIFT;
        case NKeyboard::RAlt:           return GLFW_KEY_RIGHT_ALT;
        case NKeyboard::RSystem:        return GLFW_KEY_RIGHT_SUPER;
        case NKeyboard::Menu:           return GLFW_KEY_MENU;
        case NKeyboard::LBracket:       return GLFW_KEY_LEFT_BRACKET;
        case NKeyboard::RBracket:       return GLFW_KEY_RIGHT_BRACKET;
        case NKeyboard::Semicolon:      return GLFW_KEY_SEMICOLON;
        case NKeyboard::Comma:          return GLFW_KEY_COMMA;
        case NKeyboard::Period:         return GLFW_KEY_PERIOD;
        case NKeyboard::Apostrophe:     return GLFW_KEY_APOSTROPHE;
        case NKeyboard::Slash:          return GLFW_KEY_SLASH;
        case NKeyboard::Backslash:      return GLFW_KEY_BACKSLASH;
        case NKeyboard::Grave:          return GLFW_KEY_GRAVE_ACCENT;
        case NKeyboard::Equal:          return GLFW_KEY_EQUAL;
        case NKeyboard::Dash:           return GLFW_KEY_MINUS;
        case NKeyboard::Space:          return GLFW_KEY_SPACE;
        case NKeyboard::Enter:          return GLFW_KEY_ENTER;
        case NKeyboard::Backspace:      return GLFW_KEY_BACKSPACE;
        case NKeyboard::Tab:            return GLFW_KEY_TAB;
        case NKeyboard::PageUp:         return GLFW_KEY_PAGE_UP;
        case NKeyboard::PageDown:       return GLFW_KEY_PAGE_DOWN;
        case NKeyboard::End:            return GLFW_KEY_END;
        case NKeyboard::Home:           return GLFW_KEY_HOME;
        case NKeyboard::Insert:         return GLFW_KEY_INSERT;
        case NKeyboard::Delete:         return GLFW_KEY_DELETE;
        case NKeyboard::Left:           return GLFW_KEY_LEFT;
        case NKeyboard::Right:          return GLFW_KEY_RIGHT;
        case NKeyboard::Up:             return GLFW_KEY_UP;
        case NKeyboard::Down:           return GLFW_KEY_DOWN;
        case NKeyboard::NumpadLock:     return GLFW_KEY_NUM_LOCK;
        case NKeyboard::NumpadAdd:      return GLFW_KEY_KP_ADD;
        case NKeyboard::NumpadSubtract: return GLFW_KEY_KP_SUBTRACT;
        case NKeyboard::NumpadMultiply: return GLFW_KEY_KP_MULTIPLY;
        case NKeyboard::NumpadDivide:   return GLFW_KEY_KP_DIVIDE;
        case NKeyboard::NumpadDecimal:  return GLFW_KEY_KP_DECIMAL;
        case NKeyboard::NumpadEnter:    return GLFW_KEY_KP_ENTER;
        case NKeyboard::NumpadEqual:    return GLFW_KEY_KP_EQUAL;
        case NKeyboard::Numpad0:        return GLFW_KEY_KP_0;
        case NKeyboard::Numpad1:        return GLFW_KEY_KP_1;
        case NKeyboard::Numpad2:        return GLFW_KEY_KP_2;
        case NKeyboard::Numpad3:        return GLFW_KEY_KP_3;
        case NKeyboard::Numpad4:        return GLFW_KEY_KP_4;
        case NKeyboard::Numpad5:        return GLFW_KEY_KP_5;
        case NKeyboard::Numpad6:        return GLFW_KEY_KP_6;
        case NKeyboard::Numpad7:        return GLFW_KEY_KP_7;
        case NKeyboard::Numpad8:        return GLFW_KEY_KP_8;
        case NKeyboard::Numpad9:        return GLFW_KEY_KP_9;
        case NKeyboard::PrintScreen:    return GLFW_KEY_PRINT_SCREEN;
        case NKeyboard::CapsLock:       return GLFW_KEY_CAPS_LOCK;
        case NKeyboard::ScrollLock:     return GLFW_KEY_SCROLL_LOCK;
        case NKeyboard::Pause:          return GLFW_KEY_PAUSE;
        case NKeyboard::F1:             return GLFW_KEY_F1;
        case NKeyboard::F2:             return GLFW_KEY_F2;
        case NKeyboard::F3:             return GLFW_KEY_F3;
        case NKeyboard::F4:             return GLFW_KEY_F4;
        case NKeyboard::F5:             return GLFW_KEY_F5;
        case NKeyboard::F6:             return GLFW_KEY_F6;
        case NKeyboard::F7:             return GLFW_KEY_F7;
        case NKeyboard::F8:             return GLFW_KEY_F8;
        case NKeyboard::F9:             return GLFW_KEY_F9;
        case NKeyboard::F10:            return GLFW_KEY_F10;
        case NKeyboard::F11:            return GLFW_KEY_F11;
        case NKeyboard::F12:            return GLFW_KEY_F12;
        case NKeyboard::F13:            return GLFW_KEY_F13;
        case NKeyboard::F14:            return GLFW_KEY_F14;
        case NKeyboard::F15:            return GLFW_KEY_F15;
        case NKeyboard::F16:            return GLFW_KEY_F16;
        case NKeyboard::F17:            return GLFW_KEY_F17;
        case NKeyboard::F18:            return GLFW_KEY_F18;
        case NKeyboard::F19:            return GLFW_KEY_F19;
        case NKeyboard::F20:            return GLFW_KEY_F20;
        case NKeyboard::F21:            return GLFW_KEY_F21;
        case NKeyboard::F22:            return GLFW_KEY_F22;
        case NKeyboard::F23:            return GLFW_KEY_F23;
        case NKeyboard::F24:            return GLFW_KEY_F24;
        case NKeyboard::F25:            return GLFW_KEY_F25;
    }
    return GLFW_KEY_UNKNOWN;
}

static NMouse::EButton GlfwMouseCodeToMouseButton (int button) {
    switch (button) {
        case GLFW_MOUSE_BUTTON_1: return NMouse::Left;
        case GLFW_MOUSE_BUTTON_2: return NMouse::Right;
        case GLFW_MOUSE_BUTTON_3: return NMouse::Middle;
        case GLFW_MOUSE_BUTTON_4: return NMouse::Extra1;
        case GLFW_MOUSE_BUTTON_5: return NMouse::Extra2;
        case GLFW_MOUSE_BUTTON_6: return NMouse::Extra3;
        case GLFW_MOUSE_BUTTON_7: return NMouse::Extra4;
        case GLFW_MOUSE_BUTTON_8: return NMouse::Extra5;
    }
    return NMouse::Unknown;
}

static int MouseButtonToGlfwMouseCode (NMouse::EButton button) {
    switch (button) {
        case NMouse::Left:   return GLFW_MOUSE_BUTTON_1;
        case NMouse::Right:  return GLFW_MOUSE_BUTTON_2;
        case NMouse::Middle: return GLFW_MOUSE_BUTTON_3;
        case NMouse::Extra1: return GLFW_MOUSE_BUTTON_4;
        case NMouse::Extra2: return GLFW_MOUSE_BUTTON_5;
        case NMouse::Extra3: return GLFW_MOUSE_BUTTON_6;
        case NMouse::Extra4: return GLFW_MOUSE_BUTTON_7;
        case NMouse::Extra5: return GLFW_MOUSE_BUTTON_8;
    }
    return NMouse::Unknown;
}