#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std; 
using namespace sf; 


int widthX = VideoMode::getDesktopMode().width;
int heightY = VideoMode::getDesktopMode().height;



int main()
{
    RenderWindow window(VideoMode(widthX, heightY),"Booleo", Style::Fullscreen);

    
    RectangleShape shape(Vector2f(widthX,heightY));

    shape.setFillColor(Color::White);

    RectangleShape button(Vector2f(widthX / 11, heightY / 5));
    button.setFillColor(Color::Cyan);
    Font fon;
    fon.loadFromFile("Fonts/arial.ttf");
    Text t;
    t.setFont(fon);
    t.setCharacterSize(50);
    t.setFillColor(Color::Black);
    t.setPosition(Vector2f(widthX / 2, heightY / 2));
    t.setOrigin(Vector2f(100.f, 25.f));
    t.setString("FlashBANG");

    RectangleShape cardHand1(Vector2f(120.f, 150.f));
    cardHand1.setFillColor(Color::Cyan);
    cardHand1.setPosition(60.f, heightY/2);
    cardHand1.setOrigin(Vector2f(60.f,75.f));
   


    while (window.isOpen())
    {
       
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        

        window.clear();
        window.draw(shape);
       

        if (Mouse::isButtonPressed(Mouse::Left) && cardHand1.getGlobalBounds().contains(Vector2f(Mouse::getPosition())))
        {
            
                    sf::Vector2i mousePos =  sf::Mouse::getPosition(window);
                    cardHand1.setPosition((float)mousePos.x, (float)mousePos.y);
        }
        
        window.draw(cardHand1);
        window.draw(t);
        window.display();
    }

    return 0;
}

