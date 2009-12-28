#include "WindowGame.hpp"

using namespace sf;

WindowGame::WindowGame()
{
    m_window.Create(VideoMode(800, 600, 32), "LOS");
}

WindowGame::~WindowGame()
{

}

void WindowGame::Run()
{
    while(m_window.IsOpened())
    {
        while(m_window.GetEvent(mainEventManager->GetEvent()))
        {
            if(mainEventManager->GetEvent().Type == Event::Closed)
                m_window.Close();
        }
        m_window.Clear();

        m_window.Display();
    }
}
