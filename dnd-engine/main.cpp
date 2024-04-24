#include <iostream>
#include <cmath>
#include <cstdlib>
#include "network.hpp"
#include "UI.hpp"
using namespace std;

int run_window() {
    //text display timer will be used to decide how long text should be displayed for
    //text currently not working
    int textDisplayTimer = 0;

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
            if (players[i].getNumber() == mySlot) {
                if (pow(mousePosition.x - 50 - players[i].x(), 2) + pow(mousePosition.y - 50 - players[i].y(), 2) < pow(50, 2)) {
                    //moves character to mouse position if left click
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        players[i].setPosition(mousePosition.x - 50, mousePosition.y - 50);
                    }
                    //deletes character if right click
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                        //chr[i].setNumber(0);
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
            if (players[i].getNumber() != -1) {
                window.draw(players[i].drawing());
            }
        }


        //displays the window
        window.display();
        textDisplayTimer += 1;
    }
    return 0;
}

int main() {
	char input;
	cout << "Start in Server/Client mode: (Type s/c)";
	cin >> input;
	if (input == 's') {
		Server dm("Dungeon Master");
		dm.start();
        run_window();
        /*
		char message[MAX_MSG_LEN];
		cin.getline(message, MAX_MSG_LEN);
		while (message[0] != '/') {
			if (message[0] == '~') {
				dm.kick((char[MAX_NAME_LEN])"bob");
			}
			dm.send_msg(message);
			cin.getline(message, MAX_MSG_LEN);
		}*/
        dm.shutdown();
	}
	else if (input == 'c') {
		char username[MAX_NAME_LEN];
		cout << "Input a username: ";
		cin.ignore();
		cin.getline(username, MAX_NAME_LEN);
		Client player(username);
		string ip = "127.0.0.1";
		//cout << "Input IP to connect to: ";
		//cin >> ip;
		//cin.ignore();
		player.join(ip);
        run_window();
        /*
		char message[MAX_MSG_LEN];
		cin.getline(message, MAX_MSG_LEN);
		while (message[0] != '/') {
			player.send_msg(message);
			cin.getline(message, MAX_MSG_LEN);
		}*/
		player.disconnect();
	}
    
	return 0;
}