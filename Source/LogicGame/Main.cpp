#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

using namespace std; 
using namespace sf; 


int widthX = VideoMode::getDesktopMode().width;
int heightY = VideoMode::getDesktopMode().height;
const Vector2f cardSize(widthX/16, heightY/6.75);
struct Card
{
    Card() 
	{
        card.setSize(cardSize);
        card.setFillColor(Color::Cyan);
        card.setOutlineThickness(1.f);
        card.setOutlineColor(Color::Black);
		card.setOrigin(Vector2f(cardSize.x/2, cardSize.y/2));
    }
    RectangleShape card;
    bool selected = false;
};

struct Slot
{
	Slot()
	{
		slot.setSize(cardSize);
		slot.setFillColor(Color::Yellow);
		slot.setOutlineThickness(1.f);
		slot.setOutlineColor(Color::Black);
	}
	Card* currentCard = nullptr;
	RectangleShape slot;
	bool full = false;
	string card = "";
};

int main()
{
    RenderWindow window(VideoMode(widthX, heightY),"Booleo", Style::Fullscreen);

    RectangleShape background(Vector2f(widthX,heightY));

    background.setFillColor(Color::White);

	Card* currentCard = nullptr;
	vector<Card> cards;
	bool dragging = false;
	for (int i = 0; i < 21; ++i) {
		auto card = new Card;
		card->card.setPosition(Vector2f(widthX-100.f, heightY/2));
		cards.push_back(*card);
	}

	vector<Slot> slots;
	float startW = widthX / 2 - (3.0 * cardSize.x + 2.5 * widthX / 113);
	for (float i = 0, Y = heightY / 63.5; i < 7; i++,Y += cardSize.y + heightY / 63.5)
	{
		for (float j = 0, X = startW; j < 6 - i; j++, X += cardSize.x + widthX/113)
		{
			auto slot = new Slot;
			slot->slot.setPosition(Vector2f(X, Y));
			slots.push_back(*slot);
		}
		startW += (cardSize.x + widthX / 113) / 2;
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
			for (auto& it : slots) {
				if (it.slot.getGlobalBounds().contains(mpos) && currentCard == nullptr && it.full) 
				{
					it.full = false;
					currentCard = it.currentCard;
					it.currentCard = nullptr;
					break;
				}
			}
			for (auto& it : cards) {
				if (it.card.getGlobalBounds().contains(mpos) && currentCard == nullptr) {
					it.selected = true;
					currentCard = &it;
					break;
				}
			}
		}
		else if(!(Mouse::isButtonPressed(Mouse::Left))){

			if (dragging && currentCard != nullptr)
			{
				for (int i = 0; i < slots.size(); i++)
				{
					if (slots[i].slot.getGlobalBounds().contains(mpos) && !slots[i].full)
					{
						currentCard->card.setPosition(slots[i].slot.getPosition().x + cardSize.x / 2, slots[i].slot.getPosition().y + cardSize.y / 2);
						slots[i].full = 1;
						slots[i].currentCard = currentCard;
						break;
					}
				}
			}

			dragging = false;
			if (currentCard) 
			{
				currentCard->selected = false;
				currentCard = nullptr;
			}
		}

		if (dragging && currentCard != nullptr) 
		{
			currentCard->card.setPosition(mpos.x, mpos.y);
		}

        window.clear();
        window.draw(background);
		for (size_t i = 0; i < slots.size(); i++) {
			window.draw(slots[i].slot);
		}
		for (auto i = cards.size() - 1; i != -1; i--) {
			window.draw(cards[i].card);
		}
        window.display();
    }   
}