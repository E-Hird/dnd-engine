#ifndef CLIENT_H
#define CLIENT_H

#include <print>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>
using namespace std;

class Client {
public:
	string name;
	SOCKET clientSocket;
	WSADATA wsaData;
	Client(string n);
	int join(string IP);
};


#endif // !CLIENT_H