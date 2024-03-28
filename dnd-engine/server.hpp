#ifndef SERVER_H
#define SERVER_H

#include <print>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
using namespace std;

class Server {
public:
	string name;
	static const int size = 9; // Number of clients allowed to join
	SOCKET listenSocket, acceptSocket[size];
	WSADATA wsaData;
	bool open;
	Server(string n);
	void sock_listen();
	int start();
};


#endif // !SERVER_H