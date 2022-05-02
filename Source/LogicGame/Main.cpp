#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
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
	string cardValue = "";
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
};

int main()
{	
	srand(time(NULL));

    RenderWindow window(VideoMode(widthX, heightY),"Booleo", Style::Fullscreen);

    RectangleShape background(Vector2f(widthX,heightY));

    background.setFillColor(Color::White);
	vector<string> typesOfCards = { "1or", "0or", "1and", "0and", "1xor", "0xor"};
	int c1or = 0, c0or = 0, c1and = 0, c0and = 0, c1xor = 0, c0xor = 0;
	bool limit[6] = { false, false, false, false, false, false};
	int cardRandomiser = 6, stringMover = 5;
	Card* currentCard = nullptr;
	vector<Card> cards;
	bool dragging = false;
	for (int i = 0; i < 48; i++) {
		auto card = new Card;
		card->card.setPosition(Vector2f(widthX - 100.f, heightY / 2));
		cards.push_back(*card);

		if (cardRandomiser == 0)
		{
			cardRandomiser++;
		}
		int r = rand() % cardRandomiser;
		card->cardValue = typesOfCards[r];

		if (card->cardValue == "1or")
			c1or++;
		else if (card->cardValue == "0or")
			c0or++;
		else if (card->cardValue == "1and")
			c1and++;
		else if (card->cardValue == "0and")
			c0and++;
		else if (card->cardValue == "1xor")
			c1xor++;	
		else if (card->cardValue == "0xor")
			c0xor++;

		if (c1or == 8){
			limit[0] = true;
			cardRandomiser--;
			c1or++;
		}
		if (c0or == 8){
			limit[1] = true;
			cardRandomiser--;
			c0or++;
		}
		if (c1and == 8){
			limit[2] = true;
			cardRandomiser--;
			c1and++;
		}
		if (c0and == 8){
			limit[3] = true;
			cardRandomiser--;
			c0and++;
		}
		if (c1xor == 8){
			limit[4] = true;
			cardRandomiser--;
			c1xor++;
		}
		if (c0xor == 8){
			limit[5] = true;
			cardRandomiser--;
			c0xor++;
		}

		
		for (int j = 0; j < 6 and stringMover > 0; j++)
		{
			string temp = "";
			if (limit[j]){
				temp = typesOfCards[j];
				typesOfCards[j] = typesOfCards[stringMover];
				typesOfCards[stringMover] = temp;
				stringMover--;
				limit[j] = false;
			}
		}
	
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