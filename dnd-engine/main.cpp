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

		char message[MAX_MSG_LEN];
		cin.getline(message, MAX_MSG_LEN);
		while (message[0] != '/') {
			if (message[0] == '~') {
				dm.kick((char[MAX_NAME_LEN])"bob");
			}
			dm.send_msg(message);
			cin.getline(message, MAX_MSG_LEN);
		}
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
		char message[MAX_MSG_LEN];
		cin.getline(message, MAX_MSG_LEN);
		while (message[0] != '/') {
			player.send_msg(message);
			cin.getline(message, MAX_MSG_LEN);
		}
		player.disconnect();
	}
	system("pause");
	return 0;
}