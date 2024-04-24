#ifndef Character_H
#define Character_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define MAX_NAME_LEN 64
#define CHR_NUM 10

void charcpy(char target[], char source[]);
void charcpy(char target[], std::string source);

// Player number for the current player
extern int mySlot;

//defines character class
class character {
public:
    //player number stores number of the player controlling the character
    int playerNumber;
    //stores the x and y position
    int xPos = 50;
    int yPos = 20;
    //defines if it should be shown or hidden
    bool hidden = false;
    char name[MAX_NAME_LEN] = "";
    character();
    void initChr(int number, char n[MAX_NAME_LEN]);


    //returns a shape for the object
    sf::CircleShape drawing();

    int getNumber();

    void setPosition(int x, int y);

    int x();

    int y();
};

extern character players[CHR_NUM];

#endif