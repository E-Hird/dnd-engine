#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


//used for buttons
class button {
    int xPos = 1000;
    int yPos = 20;
    int xSize = 50.f;
    int ySize = 50.f;
    sf::Texture texture;
public:
    //checks if the button is clicked
    bool isClicked(int mousex, int mousey, bool mouseClicked);

    //assigns a texture
    void assignTexture(const sf::Texture textureI);

    //returns the shape so it can be drawn
    sf::RectangleShape drawing();

    void setPosition(int x, int y);
};

#endif // !UI_H