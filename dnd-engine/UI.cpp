#include "ui.hpp"

bool button::isClicked(int mousex, int mousey, bool mouseClicked) {
    if ((mousex < xPos + xSize) && (mousex > xPos) && (mousey < yPos + ySize) && (mousey > yPos) && mouseClicked) {
        return true;
    }
}

void button::assignTexture(const sf::Texture textureI) {
    texture = textureI;
}

sf::RectangleShape button::drawing() {
    sf::RectangleShape rectangle(sf::Vector2f(xSize, ySize));

    rectangle.setPosition(xPos, yPos);
    rectangle.setTexture(&texture);
    return rectangle;
}

void button::setPosition(int x, int y) {
    xPos = x;
    yPos = y;
}