#include "Window.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "Core/CoreDebug.h"
#include "InputSystem.h"

CWindow::CWindow () {
    m_window = new sf::RenderWindow;
    Assert_(m_window != nullptr);
}

CWindow::~CWindow () {
    if (m_window) {
        delete m_window;
        m_window = nullptr;
    }
}

void CWindow::Create (const SWindowContext& context) {
    uint32_t sfWindowStyle = 0;
    sfWindowStyle |= (context.m_style & e_windowStyleTitlebar) ? sf::Style::Titlebar : 0;
    sfWindowStyle |= (context.m_style & e_windowStyleResize) ? sf::Style::Resize : 0;
    sfWindowStyle |= (context.m_style & e_windowStyleClose) ? sf::Style::Close : 0;
    sfWindowStyle |= (context.m_style & e_windowStyleFullscreen) ? sf::Style::Fullscreen : 0;
    m_window->create(
        sf::VideoMode(context.m_width, context.m_height, context.m_bitsPerPixel),
        sf::String(context.m_title),
        sfWindowStyle
    );
}

bool CWindow::IsOpen () const {
    return m_window->isOpen();
}

void CWindow::Close () {
    m_window->close();
}

void CWindow::ProcessInput (CInputSystem& input) {
    sf::Event event;
    while (m_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            Close();
        }
    }
}

void CWindow::Clear () {
    m_window->clear();
}

void CWindow::Display () {
    m_window->display();
}