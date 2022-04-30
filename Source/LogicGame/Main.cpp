#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std; 
using namespace sf; 


int widthX = VideoMode::getDesktopMode().width;
int heightY = VideoMode::getDesktopMode().height;

struct Card
{
    Card(Vector2f size) {
        card.setSize(size);
        card.setFillColor(Color::Cyan);
        card.setOutlineThickness(1.f);
        card.setOutlineColor(Color::Black);
		card.setOrigin(Vector2f(25.f, 25.f));
    }
    RectangleShape card;
    bool selected = false;
};

int main()
{
	srand((unsigned)time(nullptr));
    RenderWindow window(VideoMode(widthX, heightY),"Booleo", Style::Fullscreen);

    RectangleShape background(Vector2f(widthX,heightY));

    background.setFillColor(Color::White);
    Font fon;
    fon.loadFromFile("Fonts/arial.ttf");
    Text t;
    t.setFont(fon);
    t.setCharacterSize(50);
    t.setFillColor(Color::Black);
    t.setPosition(Vector2f(widthX / 2, heightY / 2));
    t.setOrigin(Vector2f(100.f, 25.f));
	t.setString("FlashBANG");


	Card* currentCard = nullptr;
	vector<Card> cards;
	bool dragging = false;
	for (int i = 0; i < 21; ++i) {
		auto card = new Card({ 120.f,170.f });
		card->card.setPosition(rand() % 1100 + 100, rand() % 650 + 100);
		cards.push_back(*card);

	}
    while (window.isOpen())
    {
       
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
		auto mpos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (Mouse::isButtonPressed(Mouse::Left) && !dragging) 
		{
			dragging = true;
				for (auto& it : cards) {
					if (it.card.getGlobalBounds().contains(mpos.x, mpos.y) && currentCard == nullptr) {
						it.selected = true;
						currentCard = &it;
						break;
					}
				}
		}
		else if(!(Mouse::isButtonPressed(Mouse::Left))){

			dragging = false;
			if (currentCard) 
			{
				currentCard->selected = false;
				currentCard = nullptr;
			}
		}

		if (dragging == true) {
			for (int i = 0; i < cards.size(); i++) {
				if (cards[i].selected)
					cards[i].card.setPosition(mpos.x, mpos.y);

			}
		}
        window.clear();
        window.draw(background);
		for (int i = 0; i < cards.size(); i++) {
			window.draw(cards[i].card);
		}
        window.draw(t);
        window.display();
    }   
}