#include "Widget.hpp"

using namespace sf;

Widget::Widget() : m_position(0,0), m_size(0,0)
{
    m_parent = NULL;
}

Widget::~Widget()
{

}

void Widget::AddWidget(Widget *widget)
{
    m_widgets.push_back(widget);
    m_widgets.back()->m_parent = this;
}

sf::Sprite Widget::Show()
{
    m_image.Clear();

    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            m_image.Draw((*i)->Show());

    m_image.Display();

    m_sprite.SetImage(m_image.GetImage());
    return  (m_sprite);
}

void Widget::Update()
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->Update();
}

/*void Widget::UpdateGlobalPosition()
{
    for( std::vector<Widget*>::iterator i = m_widgets.begin();
         i != m_widgets.end();
         ++i )
            (*i)->SetGlobalPosition(m_position_global.x + (*i)->GetPosition().x,
                                    m_position_global.y + (*i)->GetPosition().y);
}
*/
void Widget::SetGeometry(int x, int y, int w, int h)
{
    m_position.x        = x;
    m_position.y        = y;
    m_size.x            = w;
    m_size.y            = h;

    m_sprite.SetPosition(x, y);
    m_sprite.Resize(w, h);

    //UpdateGlobalPosition();
}

void Widget::SetPosition(int x, int y)
{
    m_position.x        = x;
    m_position.y        = y;

    m_sprite.SetPosition(x, y);

    //UpdateGlobalPosition();
}

/*void Widget::SetGlobalPosition(int x, int y)
{
    m_position_global.x = x;
    m_position_global.y = y;

    //UpdateGlobalPosition();
}
*/

const sf::Vector2i &Widget::GetSize()
{
    return (m_size);
}

const sf::Vector2i &Widget::GetPosition()
{
    return (m_position);
}

sf::Vector2i Widget::GetGlobalPosition()
{
    sf::Vector2i pos = m_position;

    if(m_parent != NULL)
    {
        pos.x   = m_position.x + m_parent->GetGlobalPosition().x;
        pos.y   = m_position.y + m_parent->GetGlobalPosition().y;
    }

    return (pos);
}
