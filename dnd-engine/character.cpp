#include "character.hpp"
#include "network.hpp"
using namespace std;

//charcopy(target, source) to make target = source (utility function)
void charcpy(char target[], char source[]) {
	if (sizeof(target) == sizeof(source)) {
		switch (sizeof(target)) {
		case MAX_NAME_LEN / 8:
			for (int i = 0; i < MAX_NAME_LEN; i++) {
				target[i] = source[i];
			}
			break;
		case MAX_MSG_LEN / 8:
			for (int i = 0; i < MAX_MSG_LEN; i++) {
				target[i] = source[i];
			}
			break;
		default:
			throw exception("unacceptable array length");
			break;
		}
	}
	else {
		throw exception("arrays must be same length");
	}
}
void charcpy(char target[], string source) {
	if (sizeof(target) * 8 > source.length()) {
		for (int i = 0; i < source.length(); i++) {
			target[i] = source[i];
		}
		target[source.length()] = '\0';
	}
	else {
		throw exception("string must be shorter than array length");
	}
}

int mySlot = 0;

// character functions
character::character() {
    playerNumber = -1;
}

void character::initChr(int number,char n[64]) {
    charcpy(name, n);
    playerNumber = number;
}

sf::CircleShape character::drawing() {
    sf::CircleShape shape(50.f);
    if (playerNumber == mySlot) {
        shape.setFillColor(sf::Color::Red);
    }
    else {
        shape.setFillColor(sf::Color::Cyan);

    }
    shape.setPosition(xPos, yPos);
    return shape;
}

void character::setNumber(int n) {
	playerNumber = n;
}

int character::getNumber() {
    return playerNumber;
}

void character::setPosition(int x, int y) {
    xPos = x;
    yPos = y;
}
int character::x() {
    return xPos;
}
int character::y() {
    return yPos;
}


character players[CHR_NUM];