#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/CoreDebug.h"

static NKeyboard::EKey GlfwKeyCodeToKeyboardKey (int key);
static NMouse::EButton GlfwMouseCodeToMouseButton (int button);

static void GlfwFramebufferSizeCallback (GLFWwindow* window, int width, int height);
static void GlfwKeyCallback (GLFWwindow* window, int key, int scanCode, int action, int mode);
static void GlfwMouseButtonCallback (GLFWwindow* window, int button, int action, int mods);
static void GlfwMouseMoveCallback (GLFWwindow* window, double xpos, double ypos);
static void GlfwMouseEnterCallback (GLFWwindow* window, int entered);
static void GlfwMouseScrollCallback (GLFWwindow* window, double xoffset, double yoffset);
static void GlfwResizeCallback (GLFWwindow* window, int width, int height);
static void GlfwCloseCallback (GLFWwindow* window);

CWindow::CWindow (const SWindowContext& context) {
    Create(context);
}

CWindow::~CWindow () {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void CWindow::Create (const SWindowContext& context) {
    int glfwInitSuccess = glfwInit();
    Assert_(glfwInitSuccess);
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
    Assert_(m_window != nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwMakeContextCurrent(m_window);
    int gladInitSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    Assert_(gladInitSuccess);
    glViewport(0, 0, context.m_width, context.m_height);
    glfwSetFramebufferSizeCallback(m_window, GlfwFramebufferSizeCallback);
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
        return false;
    }
    inputEvent = m_inputQueue.back();
    m_inputQueue.pop();
    return true;
}

void CWindow::PushInput (const CInputEvent& inputEvent) {
    m_inputQueue.push(inputEvent);
}

void CWindow::Clear () {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CWindow::Display () {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
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
    if (action == GLFW_PRESS) {
        event.m_type = CInputEvent::EEventType::KeyPressed;
    }
    else if (action == GLFW_RELEASE) {
        event.m_type = CInputEvent::EEventType::KeyReleased;
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
        event.m_type = CInputEvent::EEventType::MousePressed;
    }
    else if (action == GLFW_RELEASE) {
        event.m_type = CInputEvent::EEventType::MouseReleased;
    }
    window->PushInput(event);
}

static void GlfwMouseMoveCallback (GLFWwindow* glfwWindow, double xpos, double ypos) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_mouseMove.m_x = static_cast<int32_t>(xpos);
    event.m_mouseMove.m_y = static_cast<int32_t>(ypos);
    event.m_type = CInputEvent::EEventType::MouseMoved;
    window->PushInput(event);
}

static void GlfwMouseEnterCallback (GLFWwindow* glfwWindow, int entered) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    if (entered) {
        event.m_type = CInputEvent::EEventType::MouseEntered;
    }
    else {
        event.m_type = CInputEvent::EEventType::MouseLeft;
    }
    window->PushInput(event);
}

static void GlfwMouseScrollCallback (GLFWwindow* glfwWindow, double xoffset, double yoffset) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_mouseScroll.m_x = static_cast<int32_t>(xoffset);
    event.m_mouseScroll.m_y = static_cast<int32_t>(yoffset);
    event.m_type = CInputEvent::EEventType::MouseScrolled;
    window->PushInput(event);
}

static void GlfwResizeCallback (GLFWwindow* glfwWindow, int width, int height) {
    CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
    CInputEvent event;
    event.m_resize.m_width = width;
    event.m_resize.m_height = height;
    event.m_type = CInputEvent::EEventType::Resized;
    window->PushInput(event);
}

static void GlfwCloseCallback (GLFWwindow* glfwWindow) {
    if (glfwWindowShouldClose(glfwWindow)) {
        CWindow* window = static_cast<CWindow*>(glfwGetWindowUserPointer(glfwWindow));
        CInputEvent event;
        event.m_type = CInputEvent::EEventType::Closed;
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