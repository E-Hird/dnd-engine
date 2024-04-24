#include "UI.hpp"
#include "character.hpp"
#include <cmath>
#include <cstdlib> 
#include <iostream>

using namespace std;




int Main()
{
    //text display timer will be used to decide how long text should be displayed for
    //text currently not working
    int textDisplayTimer=0;
        
    //loads a font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        cout << "Hello World!";
    }
    //loads two textures
    //texture1 for background
    sf::Texture texture;
    //texture2 for stop button for music
    sf::Texture texture2;
    texture2.loadFromFile("stop.png");
    texture.loadFromFile("image.png");

    //sets a sprite with the first texture for the background
    sf::Sprite background;
    background.setTexture(texture);

    //sets text attributes
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::Green);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    //creates an array of 10 characters
    //characters with player number 0 will not appear
    //characters with player number 1 will be controlled by the user
    
    character chr[CHR_NUM];
    for (int i = 0; i < CHR_NUM; i++) {
    //    chr[i].setNumber(0);
    }
    //sets some numbers of characters
    //chr[1].setNumber(1);
    //chr[2].setNumber(1);
    int nextCharacter = 3;

    //creates the music stop button
    button button1;
    button1.assignTexture(texture2);

    //creates a button to add new characters
    button addNewCharacterButton;
    addNewCharacterButton.setPosition(10, 10);
    
    //holds if the mouse has been clicked
    bool mouseClicked=false;

    //stores when the mouse is released
    bool mouseReleased = false;

    //creates the window
    sf::RenderWindow window(sf::VideoMode(1200, 700), "SFML");

    //defines a bar at the top of the screen
    sf::RectangleShape bar(sf::Vector2f(1200, 80));
    bar.setPosition(0, 0);
    bar.setFillColor(sf::Color::Blue);

    //loads music (currently not uploaded) (has been tested)
    sf::Music music;

    if (!music.openFromFile("music.wav"))
        return -1;
    sf::Music music2;
    if (!music2.openFromFile("music2.wav"))
        return -1;

    //main loop
    while (window.isOpen())
    {   
        //checks if the mouse has been clicked
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


        //checks for window closes
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //plays music based on num key pressed
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
        
        //sets mouse position
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        //checks if mouse over character and player owns character
        for (int i = 0; i < CHR_NUM; i++) {
            if (chr[i].getNumber() == 1) {
                if (pow(mousePosition.x - 50 - chr[i].x(), 2) + pow(mousePosition.y - 50 - chr[i].y(), 2) < pow(50, 2)) {
                    //moves character to mouse position if left click
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        chr[i].setPosition(mousePosition.x - 50, mousePosition.y - 50);
                    }
                    //deletes character if right click
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        //chr[i].setNumber(0);
                    }
                }
            }
        }
        
        
        //stops music if stop button is clicked
        if (button1.isClicked(mousePosition.x, mousePosition.y, mouseReleased) == true) {
            music.stop();
            music2.stop();
        }

        //adds a new character when button is clicked and sorts out some possible issues
        if (addNewCharacterButton.isClicked(mousePosition.x, mousePosition.y, mouseReleased) == true) {
            //chr[nextCharacter].setNumber(1);
            nextCharacter += 1;
            if (nextCharacter > CHR_NUM) {
                for (int i = 0; i < CHR_NUM; i++) {
                    if (chr[i].getNumber() == 0) {
                        chr[i].setPosition(50, 20);
                        nextCharacter = i;
                    }
                }
            }
        }
      
        
        //clears window
        window.clear();
        //draws background
        window.draw(background);
        //draws the bar
        window.draw(bar);
        //tries to draw text
        if (textDisplayTimer < 10000) {
            window.draw(text);
        }

        //draws the characters
        for (int i = 0; i < CHR_NUM; i++) {
            if (chr[i].getNumber()!=0) {
                window.draw(chr[i].drawing());
            }
        }
        //draws the buttons
        window.draw(button1.drawing());
        window.draw(addNewCharacterButton.drawing());
        
        //displays the window
        window.display();
        textDisplayTimer += 1;
    }

    return 0;
}