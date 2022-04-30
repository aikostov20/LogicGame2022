#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std; 
using namespace sf; 


int widthX = sf::VideoMode::getDesktopMode().width;
int heightY = sf::VideoMode::getDesktopMode().height;


int main()
{
    RenderWindow window(sf::VideoMode(0.f,0.f),"Booleo", Style::Fullscreen);
    RectangleShape shape(Vector2f(widthX,heightY));
    shape.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
       
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

