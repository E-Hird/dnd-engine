#include <iostream>
#include "network.hpp"
using namespace std;

int main() {
	char input;
	cout << "Start in Server/Client mode: (Type s/c)";
	cin >> input;
	if (input == 's') {
		Server dm("Dungeon Master");
		dm.start();
	}
	else if (input == 'c') {
		char username[MAX_NAME_LEN];
		cout << "Input a username: ";
		cin.ignore();
		cin.getline(username, MAX_NAME_LEN);
		Client player(username);
		string ip;
		cout << "Input IP to connect to: ";
		cin >> ip;
		cin.ignore();
		player.join(ip);
		char message[MAX_MSG_LEN];
		while (message[0] != '/') {
			cin.getline(message, MAX_MSG_LEN);
			player.send_msg(message);
		}
		player.disconnect();
	}
	system("pause");
	return 0;
}