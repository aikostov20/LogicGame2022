#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <algorithm>
#include <vector>
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
		card.setOrigin(Vector2f(cardSize.x/2, cardSize.y/2));
    }
    RectangleShape card;
    bool selected = false, result = false, locked = false;
	Vector2f lastPos;
	Texture texture;
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
	}
	Card* currentCard = nullptr;
	RectangleShape slot;
	Texture texture;
	int index = 0, level = 0, cardsIn = 0;
	bool pHand = false, full = false, locked = false, deck = false;

};

void mainMenu();

void PVP()
{
	srand(time(NULL));
	bool playerTurn = 0;
	Texture BGtexture;
	BGtexture.loadFromFile("../../Images/BGtexture.png");
	BGtexture.setSmooth(true);
	background.setTexture(&BGtexture);

	RectangleShape ExitButton(Vector2f(widthX / 20, heightY / 22));
	ExitButton.setFillColor(Color::Red);
	ExitButton.setPosition(Vector2f(widthX / 50, heightY / 40));
	ExitButton.setOutlineThickness(2.f);
	ExitButton.setOutlineColor(Color::Red);

	RectangleShape TTButton(Vector2f(widthX / 20, heightY / 22));
	TTButton.setFillColor(Color::Transparent);
	TTButton.setPosition(Vector2f(widthX-100, 20));
	TTButton.setOutlineThickness(2.f);
	TTButton.setOutlineColor(Color::Red);

	RectangleShape TruthTable(Vector2f(widthX / 5, heightY / 10));
	TruthTable.setFillColor(Color::White);
	TruthTable.setPosition(widthX+100, heightY / 2);
	TruthTable.setOrigin(Vector2f(widthX / 10, heightY / 20));


	Texture TruthTableTexture;
	TruthTableTexture.loadFromFile("../../Images/truthTable.png");
	TruthTable.setTexture(&TruthTableTexture);
	
	//Cards vector
	Card* currentCard = nullptr;
	vector<Card> cards;
	bool dragging = false;
	//Slots vector
	vector<Slot> slots;
	float startW = widthX / 2 - (3.0 * cardSize.x + 2.5 * widthX / 113);
	int lock = 0, level = 0;

	//Generate slots
	auto deck = new Slot;
	deck->cardsIn = 48;
	deck->deck = 1;
	deck->locked = 1;
	deck->slot.setPosition(Vector2f(widthX - 100.f - cardSize.x / 2, heightY / 2 - cardSize.y / 2));
	slots.push_back(*deck);
	for (int i = 0; i < 6; i++)
	{
		auto card = new Card;
		card->result = rand() % 2;
		card->texture.loadFromFile("../../Images/1-0Card.png");
		card->texture.setSmooth(1);
		card->locked = true;
		card->card.setTexture(&card->texture);
		if (!card->result)
		{
			card->card.setRotation(180);
		}
		cards.push_back(*card);
	}
	for (float i = 0, Y = heightY / 63.5; i < 7; i++, Y += cardSize.y + heightY / 63.5)
	{
		for (float j = 0, X = startW; j < 6 - i; j++, X += cardSize.x + widthX / 113, lock++)
		{
			auto slot = new Slot;
			slot->slot.setPosition(Vector2f(X, Y));
			slot->index = int(i);

			if (lock < 6)
			{
				slot->locked = true;
				slot->currentCard = &cards[lock];
				slot->currentCard->card.setPosition(slot->slot.getPosition().x + cardSize.x / 2, slot->slot.getPosition().y + cardSize.y / 2);
				slot->currentCard->lastPos = slot->slot.getPosition();
				slot->currentCard->locked = true;
				slot->full = true;
			}
			slot->level = level;
			slots.push_back(*slot);
		}
		level++;
		startW += (cardSize.x + widthX / 113) / 2;
	}

	

	// Generate cards
	vector<string> typesOfCards = { "1or", "0or", "1and", "0and", "1xor", "0xor" };
	int count[6] = {0,0,0,0,0,0}, cardRandomiser = 6, curCount, i = 0;
	string temp;
	int index = 0;
	while (i < 48) {
		auto card = new Card;
		card->card.setPosition(Vector2f(widthX - 100.f, heightY / 2));
		card->lastPos = Vector2f(card->card.getPosition().x - cardSize.x/2-1, card->card.getPosition().y - cardSize.y / 2-1);

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

		(card->cardValue != "") ? deck->currentCard = card : 0;
		if (card->cardValue.size() != 0)
		{
			cards.push_back(*card);
			i++;
		}
	}
	for (int i = 6; i < cards.size(); i++)
	{
		cards[i].result = (cards[i].cardValue.substr(0, 1) == '1');
		cards[i].texture.loadFromFile("../../Images/" + cards[i].cardValue + ".png");
		cards[i].texture.setSmooth(1);
		cards[i].card.setTexture(&cards[i].texture);
	}

	for (float i = 0, Y = heightY - (heightY / 63.5 + cardSize.y); i < 5; i++, Y-=cardSize.y + heightY/63.5)
	{
		auto hand = new Slot;
		hand->pHand = 1;
		hand->slot.setPosition(widthX/113,Y);
		slots.push_back(*hand);
	}
	for (int i = 0; i < slots.size(); i++)
	{
		slots[i].texture.loadFromFile("../../Images/BGslot.png");
		slots[i].slot.setTexture(&slots[i].texture);
	}
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)){}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {mainMenu(); }

		auto mpos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (Mouse::isButtonPressed(Mouse::Left) && !dragging)
		{
			dragging = true;
			for (auto& it : slots) {
				if (it.slot.getGlobalBounds().contains(mpos) && currentCard == nullptr && it.full && !it.currentCard->locked)
				{
					it.full = false;
					it.cardsIn--;
					currentCard = it.currentCard;
					it.currentCard = nullptr;
					break;
				}
			}
			for (auto& it : cards) {
				if (it.card.getGlobalBounds().contains(mpos) && currentCard == nullptr && !it.locked){
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
						slots[i].cardsIn++;
						if (!slots[i].deck) 
						{
							slots[i].full = 1;
						}
						else if (slots[i].cardsIn == 48)
						{
							slots[i].full = 1;
						}
						slots[i].currentCard = currentCard;
						currentCard->lastPos = Vector2f(currentCard->card.getGlobalBounds().left,
							currentCard->card.getGlobalBounds().top);
						break;
						}//}
				}
				//currentCard->card.setPosition(currentCard->lastPos.x + cardSize.x/2, currentCard->lastPos.y + cardSize.y/2);
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
		}
		if (Mouse::isButtonPressed(Mouse::Left) && ExitButton.getGlobalBounds().contains(mpos))
		{
			mainMenu();

		}

		if (Mouse::isButtonPressed(Mouse::Left) && TTButton.getGlobalBounds().contains(mpos))
		{
			TruthTable.setPosition(widthX/2, heightY/2);
			
		}
		//sf::Event::MouseButtonReleased
		if (!Mouse::isButtonPressed(Mouse::Left) )
		{
			TruthTable.setPosition(widthX +200, heightY / 2);

		}
		window.clear();
		window.draw(background);
		window.draw(ExitButton); 
		window.draw(TTButton); 
		for (size_t i = 0; i < slots.size(); i++) {
			window.draw(slots[i].slot);
		}

		//Draw the cards
		for (auto i = cards.size() - 1; i != -1; i--) {
			window.draw(cards[i].card);
		}
		window.draw(TruthTable);
		window.display();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void  H2P()
{
	RectangleShape content(Vector2f(widthX /2, heightY ));
	content.setFillColor(Color::White);
	content.setOrigin(Vector2f(widthX  /2/ 2, heightY / 2));
	content.setPosition(widthX / 2, heightY /2);
	content.setOutlineThickness(3.f);
	content.setOutlineColor(Color::Black);


	RectangleShape ExitButton(Vector2f(widthX / 20, heightY / 22));
	ExitButton.setFillColor(Color::Red);
	ExitButton.setPosition(Vector2f(widthX / 50, heightY / 40));
	ExitButton.setOutlineThickness(2.f);
	ExitButton.setOutlineColor(Color::Red);

	auto mpos = window.mapPixelToCoords(Mouse::getPosition(window));
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
				mainMenu();
		}
		if (Mouse::isButtonPressed(Mouse::Left) && ExitButton.getGlobalBounds().contains(mpos))
		{
			mainMenu();

		}
		window.clear();
		window.draw(background);
		window.draw(content);

		window.display();
	}


}

void mainMenu()
{
	Texture BGtexture;
	BGtexture.loadFromFile("../../Images/BGtexture.png");
	BGtexture.setSmooth(true);
	background.setTexture(&BGtexture);


	RectangleShape PVPbutton(Vector2f(widthX/8,heightY/10));
	RectangleShape PVCbutton(Vector2f(widthX / 8, heightY / 10));
	RectangleShape H2Pbutton(Vector2f(widthX / 10, heightY / 12));
	RectangleShape Exitbutton(Vector2f(widthX / 10, heightY / 12));
	Texture pvp, pvc, h2p, exit;


	pvp.loadFromFile("../../Images/pvpButton.png");
	pvc.loadFromFile("../../Images/pvcButton.png");
	h2p.loadFromFile("../../Images/h2pButton.png");
	exit.loadFromFile("../../Images/exitButton.png");
	pvp.setSmooth(1);
	pvc.setSmooth(1);
	h2p.setSmooth(1);
	exit.setSmooth(1);


	PVPbutton.setTexture(&pvp);
	PVPbutton.setPosition(Vector2f(widthX/2,heightY/2-150));
	PVPbutton.setOrigin(Vector2f(widthX / 8 / 2, heightY / 8 / 2));

	PVCbutton.setTexture(&pvc);
	PVCbutton.setPosition(Vector2f(widthX / 2, heightY / 2-35));
	PVCbutton.setOrigin(Vector2f(widthX / 8 / 2, heightY / 8 / 2));

	H2Pbutton.setTexture(&h2p);
	H2Pbutton.setPosition(Vector2f(widthX / 2, heightY / 2 +55));
	H2Pbutton.setOrigin(Vector2f(widthX / 10 / 2, heightY / 12 / 2));

	Exitbutton.setTexture(&exit);
	Exitbutton.setPosition(Vector2f(widthX / 2, heightY / 2 + 155));
	Exitbutton.setOrigin(Vector2f(widthX / 10 / 2, heightY / 12 / 2));

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)){}
		Vector2f mpos = window.mapPixelToCoords(Mouse::getPosition(window));

		if (Mouse::isButtonPressed(Mouse::Left) && PVPbutton.getGlobalBounds().contains(mpos))
		{
			PVP();
		}
		if (Mouse::isButtonPressed(Mouse::Left) && H2Pbutton.getGlobalBounds().contains(mpos))
		{
			H2P();
		}
		if (Mouse::isButtonPressed(Mouse::Left) && Exitbutton.getGlobalBounds().contains(mpos))
		{
			window.close(); 
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