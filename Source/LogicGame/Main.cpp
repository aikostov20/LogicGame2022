#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std; 
using namespace sf; 



int widthX = VideoMode::getDesktopMode().width;
int heightY = VideoMode::getDesktopMode().height;

RenderWindow window(VideoMode(widthX, heightY), "Booleo", Style::Fullscreen);
RectangleShape background(Vector2f(widthX, heightY));

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
		cardValueT.setCharacterSize(20.f);
		cardValueT.setFillColor(Color::Black);
		font.loadFromFile("Fonts/arial.ttf");
		cardValueT.setFont(font);
    }
	Font font;
	Text cardValueT;
    RectangleShape card;
    bool selected = false, result = false;
	Vector2f lastPos;
	string cardValue = "";
	bool Operation(bool a, bool b, string op)
	{
		if (op == "or") { return a | b; }
		if (op == "and") { return a & b; }
		return a ^ b; 
	}
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
	int index = 0, level = 0;
	bool pHand = false, full = false, locked = false;

};

void PVP()
{
	srand(time(NULL));
	background.setFillColor(Color::White);

	Texture BGtexture;
	BGtexture.loadFromFile("../../Images/BGtexture.png");
	BGtexture.setSmooth(true);
	background.setTexture(&BGtexture);

	RectangleShape ExitButton(Vector2f(widthX / 20, heightY / 22));
	ExitButton.setFillColor(Color::Red);
	ExitButton.setPosition(Vector2f(widthX / 50, heightY / 40));
	ExitButton.setOutlineThickness(2.f);
	ExitButton.setOutlineColor(Color::Red);

	Card* currentCard = nullptr;
	vector<Card> cards;
	bool dragging = false;

	// Generate cards
	vector<string> typesOfCards = { "1or", "0or", "1and", "0and", "1xor", "0xor" };
	int count[6] = {0,0,0,0,0,0}, cardRandomiser = 6, curCount, i = 0;
	string temp;
	while (i < 48) {
		auto card = new Card;
		card->card.setPosition(Vector2f(widthX - 100.f, heightY / 2));
		card->lastPos = Vector2f(card->card.getPosition().x - cardSize.x/2-1, card->card.getPosition().y - cardSize.y / 2-1);
		card->cardValueT.setOrigin(card->cardValueT.getLocalBounds().width / 2, card->cardValueT.getLocalBounds().height / 2);
		card->cardValueT.setPosition(Vector2f(card->card.getPosition().x - cardSize.x / 2 - 1, card->card.getPosition().y - cardSize.y / 2 - 1));

		int r = rand() % cardRandomiser;
		temp = typesOfCards[r];

		if (temp == "1or") {
			curCount = 0;
			if (count[curCount] < 8){count[curCount]++; card->cardValue = temp;}
		}
		else if (temp == "0or") {
			curCount = 1;
			if (count[curCount] < 8) { count[curCount]++; card->cardValue = temp; }
		}
		else if (temp == "1and") {
			curCount = 2;
			if (count[curCount] < 8) { count[curCount]++; card->cardValue = temp; }
		}
		else if (temp == "0and") {
			curCount = 3;
			if (count[curCount] < 8) { count[curCount]++; card->cardValue = temp; }
		}
		else if (temp == "1xor") {
			curCount = 4;
			if (count[curCount] < 8) { count[curCount]++; card->cardValue = temp; }
		}
		else {
			curCount = 5;
			if (count[curCount] < 8) { count[curCount]++; card->cardValue = temp; }
		}

		if (card->cardValue.size() != 0)
		{
			cards.push_back(*card);
			i++;
		}
	}
	for (int i = 0; i < cards.size(); i++)
	{
		cards[i].cardValueT.setString(cards[i].cardValue);
		cards[i].result = (cards[i].cardValue.substr(0, 1) == '1');
	}

	//Generate slots
	vector<Slot> slots;
	float startW = widthX / 2 - (3.0 * cardSize.x + 2.5 * widthX / 113);
	int lock = 0, level = 0;
	for (float i = 0, Y = heightY / 63.5; i < 7; i++, Y += cardSize.y + heightY / 63.5)
	{
		for (float j = 0, X = startW; j < 6 - i; j++, X += cardSize.x + widthX / 113, lock++)
		{
			auto slot = new Slot;
			slot->slot.setPosition(Vector2f(X, Y));
			slot->index = int(i);
			
			if (lock < 6)
				slot->locked = true;
			slot->level = level;

			slots.push_back(*slot);
		}
		level++;
		startW += (cardSize.x + widthX / 113) / 2;
	}

	for (float i = 0, Y = heightY - (heightY / 63.5 + cardSize.y); i < 5; i++, Y-=cardSize.y + heightY/63.5)
	{
		auto hand = new Slot;
		hand->pHand = 1;
		hand->slot.setPosition(widthX/113,Y);
		slots.push_back(*hand);
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
		else if (!(Mouse::isButtonPressed(Mouse::Left))) {

			if (dragging && currentCard != nullptr)
			{
				for (int i = 0; i < slots.size(); i++)
				{
					if (slots[i].slot.getGlobalBounds().contains(mpos) && !slots[i].full && !slots[i].locked)
					{
						//Operation checking

						//if (slots[i - (7 - slots[i].level)].full && slots[i - (6 - slots[i].level)].full && 
							//slots[i].currentCard->Operation(slots[i - (7 - slots[i].level)].currentCard->result,
							//slots[i - (6 - slots[i].level)].currentCard->result,
							//slots[i].currentCard->cardValue.substr(1))){

						currentCard->card.setPosition(slots[i].slot.getPosition().x + cardSize.x / 2,
							slots[i].slot.getPosition().y + cardSize.y / 2);
						slots[i].full = 1;
						slots[i].currentCard = currentCard;
						currentCard->cardValueT.setPosition(currentCard->card.getGlobalBounds().left,
							currentCard->card.getGlobalBounds().top);
						currentCard->lastPos = Vector2f(currentCard->card.getGlobalBounds().left,
							currentCard->card.getGlobalBounds().top);
						break;
						}//}
				}
				currentCard->card.setPosition(currentCard->lastPos.x + cardSize.x/2+1, currentCard->lastPos.y + cardSize.y/2+1);
				currentCard->cardValueT.setPosition(currentCard->card.getGlobalBounds().left,
				currentCard->card.getGlobalBounds().top);
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
			currentCard->card.setPosition(mpos);
			currentCard->cardValueT.setPosition(currentCard->card.getGlobalBounds().left, 
												currentCard->card.getGlobalBounds().top);
		}
		if (Mouse::isButtonPressed(Mouse::Left) && ExitButton.getGlobalBounds().contains(mpos))
		{
			window.close();

		}

		window.clear();
		window.draw(background);
		window.draw(ExitButton); 
		for (size_t i = 0; i < slots.size(); i++) {
			window.draw(slots[i].slot);
		}

		//Draw the cards
		for (auto i = cards.size() - 1; i != -1; i--) {
			window.draw(cards[i].card);
			window.draw(cards[i].cardValueT);
		}
		window.display();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void mainMenu()
{
	background.setFillColor(Color::White);

	RectangleShape PVPbutton(Vector2f(widthX/8,heightY/10));
	RectangleShape PVCbutton(Vector2f(widthX / 8, heightY / 10));
	RectangleShape H2Pbutton(Vector2f(widthX / 10, heightY / 12));
	RectangleShape Exitbutton(Vector2f(widthX / 10, heightY / 12));
	
	PVPbutton.setFillColor(Color::Blue);
	PVPbutton.setPosition(Vector2f(widthX/2,heightY/2-150));
	PVPbutton.setOrigin(Vector2f(widthX / 8 / 2, heightY / 8 / 2));
	PVPbutton.setOutlineThickness(2.f);
	PVPbutton.setOutlineColor(Color::Blue);


	PVCbutton.setFillColor(Color::Cyan);
	PVCbutton.setPosition(Vector2f(widthX / 2, heightY / 2-50));
	PVCbutton.setOrigin(Vector2f(widthX / 8 / 2, heightY / 8 / 2));
	PVCbutton.setOutlineThickness(2.f);
	PVCbutton.setOutlineColor(Color::Cyan);


	H2Pbutton.setFillColor(Color::Green);
	H2Pbutton.setPosition(Vector2f(widthX / 2, heightY / 2 +50));
	H2Pbutton.setOrigin(Vector2f(widthX / 10 / 2, heightY / 12 / 2));
	H2Pbutton.setOutlineThickness(2.f);
	H2Pbutton.setOutlineColor(Color::Green);

	Exitbutton.setFillColor(Color::Red);
	Exitbutton.setPosition(Vector2f(widthX / 2, heightY / 2 + 150));
	Exitbutton.setOrigin(Vector2f(widthX / 10 / 2, heightY / 12 / 2));
	Exitbutton.setOutlineThickness(2.f);
	Exitbutton.setOutlineColor(Color::Red);

	
	while (window.isOpen())
	{
		Vector2f mpos = window.mapPixelToCoords(Mouse::getPosition(window));

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Mouse::isButtonPressed(Mouse::Left) && PVPbutton.getGlobalBounds().contains(mpos))
		{
			PVP();
			
		}

		if (Mouse::isButtonPressed(Mouse::Left) && Exitbutton.getGlobalBounds().contains(mpos))
		{
			window.close(); 
			exit(0);
			
		}

		window.clear();
		window.draw(background);
		
		window.draw(PVPbutton);
		window.draw(PVCbutton);
		window.draw(H2Pbutton); 
		window.draw(Exitbutton);

		window.display(); 
	}

}



int main()
{	
	mainMenu();
}