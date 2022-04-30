#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std; 
using namespace sf; 


int widthX = VideoMode::getDesktopMode().width;
int heightY = VideoMode::getDesktopMode().height;


int main()
{
    RenderWindow window(VideoMode(0.f,0.f),"Booleo", Style::Fullscreen);
    RectangleShape shape(Vector2f(widthX,heightY));
    RectangleShape button(Vector2f(widthX / 11, heightY / 5));
    button.setFillColor(Color::Cyan);
    Font fon;
    fon.loadFromFile("Fonts/arial.ttf");
    Text t;
    t.setFont(fon);
    t.setCharacterSize(50);
    t.setFillColor(Color::Black);
    t.setPosition(Vector2f(widthX / 2, heightY / 2));
    t.setOrigin(Vector2f(100.f,25.f));
    t.setString("FlashBANG");
    shape.setFillColor(Color::White);
    while (window.isOpen())
    {
       
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        

        window.clear();
        window.draw(shape);
        window.draw(button);
        if (Mouse::isButtonPressed(Mouse::Left) && button.getLocalBounds().contains(Vector2f(Mouse::getPosition())))
        {
            window.draw(t);
        }
        window.display();
    }

    return 0;
}

