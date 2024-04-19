#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <cstdlib> 
#include <iostream>

using namespace std;


class character {
    int playerNumber;
    int xPos=50;
    int yPos=20;
    bool hidden = false;
    public:
        character() {
            playerNumber = 0;
        }
        void setNumber(int number){
            playerNumber = number;
        }

        void randomMove() {
            xPos += (rand() % 3) - 1;
            yPos += (rand() % 3) - 1;
        }

        sf::CircleShape drawing() {
            sf::CircleShape shape(50.f);
            if (playerNumber == 1) {
                shape.setFillColor(sf::Color::Red);
            }
            else {
                shape.setFillColor(sf::Color::Cyan);

            }
            shape.setPosition(xPos, yPos);
            return shape;
        }

        int getNumber() {
            return playerNumber;
        }

        void setPosition(int x, int y) {
            xPos = x;
            yPos = y;
        }

        int x() {
            return xPos;
        }

        int y() {
            return yPos;
        }
};

class button {
    int xPos=1000;
    int yPos=20;
    int xSize=50.f;
    int ySize=50.f;
    sf::Texture texture;
    public:
        bool isClicked(int mousex, int mousey,bool mouseClicked) {
            if ((mousex<xPos+xSize) && (mousex>xPos) && (mousey<yPos + ySize) && (mousey>yPos) && mouseClicked) {
                return true;
            }
        }

        void assignTexture(const sf::Texture textureI) {
            texture = textureI;

        }
        
        sf::RectangleShape drawing() {
            sf::RectangleShape rectangle(sf::Vector2f(xSize, ySize));

            rectangle.setPosition(xPos, yPos);
            rectangle.setTexture(&texture);
            return rectangle;
        }

        void setPosition(int x, int y) {
            xPos = x;
            yPos = y;
        }
};



int main()
{
    int textDisplayTimer=0;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cout << "Hello World!";
    }
    sf::Texture texture;
    sf::Texture texture2;
    texture2.loadFromFile("stop.png");
    if (!texture.loadFromFile("image.png"))
    {
        cout << "Error!";
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Green);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    character chr[30];
    for (int i = 0; i < 30; i++) {
        chr[i].setNumber(0);
    }
    
    chr[1].setNumber(1);
    chr[2].setNumber(1);
    int nextCharacter = 3;

    button button1;
    button1.assignTexture(texture2);

    button addNewCharacterButton;
    addNewCharacterButton.setPosition(10, 10);
    
    bool mouseClicked=false;

    sf::RenderWindow window(sf::VideoMode(1200, 700), "SFML");

    sf::RectangleShape bar(sf::Vector2f(1200, 80));
    bar.setPosition(0, 0);
    bar.setFillColor(sf::Color::Blue);

    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Green);
    sf::Music music;

    if (!music.openFromFile("music.wav"))
        return -1; // error
    sf::Music music2;
    if (!music2.openFromFile("music2.wav"))
        return -1; // error

    bool mouseReleased = false;

    while (window.isOpen())
    {   
        mouseReleased = false;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mouseClicked = true;
        }
        else {
            if (mouseClicked == true) {
                mouseReleased = true;
            }
            mouseClicked = false;
        }

        for (int i = 0; i < 30; i++) {
            if (chr[i].getNumber() == 2) {
                chr[i].randomMove();
            }
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
        {
            shape.move(0.04f, 0.f);
            text.setString("Hello world");
            textDisplayTimer = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
        {
            shape.move(-0.04f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num1))
        {
            music.play();
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num2))
        {
            music2.play();

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Num0))
        {
            music.stop();
            music2.stop();

        }
        
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f shapePosition = shape.getPosition();
        for (int i = 0; i < 30; i++) {
            if (chr[i].getNumber() == 1) {
                if (pow(mousePosition.x - 50 - chr[i].x(), 2) + pow(mousePosition.y - 50 - chr[i].y(), 2) < pow(50, 2)) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        chr[i].setPosition(mousePosition.x - 50, mousePosition.y - 50);
                    }
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        chr[i].setNumber(0);
                    }
                }
            }
        }
        
        
        
        if (button1.isClicked(mousePosition.x, mousePosition.y, mouseReleased) == true) {
            music.stop();
            music2.stop();
        }

        if (addNewCharacterButton.isClicked(mousePosition.x, mousePosition.y, mouseReleased) == true) {
            chr[nextCharacter].setNumber(1);
            nextCharacter += 1;
            if (nextCharacter > 30) {
                for (int i = 0; i < 30; i++) {
                    if (chr[i].getNumber() == 0) {
                        chr[i].setPosition(50, 20);
                        nextCharacter = i;
                    }
                }
            }
        }
      
        
        
        
        window.clear();
        window.draw(sprite);
        window.draw(bar);
        if (textDisplayTimer < 10000) {
            window.draw(text);
        }
        for (int i = 0; i < 30; i++) {
            if (chr[i].getNumber()!=0) {
                window.draw(chr[i].drawing());
            }
        }
        window.draw(button1.drawing());
        window.draw(text);
        window.draw(addNewCharacterButton.drawing());
        
        text.setPosition(1, 1);
        window.display();
        textDisplayTimer += 1;
    }

    return 0;
}