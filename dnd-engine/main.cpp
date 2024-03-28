#include <iostream>
#include "server.hpp"
#include "client.hpp"
using namespace std;

int main() {
	char input;
	cout << "Start in Server/Client mode: (Type s/c)";
	cin >> input;
	if (input == 's') {
		Server dm("DM");
		dm.start();
	}
	else if (input == 'c') {
		Client player("Hothrall");
		player.join("127.0.0.1");
	}
	system("pause");
	return 0;
}