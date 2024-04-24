#ifndef NETWORK_H
#define NETWORK_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "character.hpp"
#include <print>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Mswsock.h>
#include <tchar.h>
#include <thread>
#include <fstream>


#define MAX_MSG_LEN 512
using namespace std;

enum dataType { CONNECT, DISCONNECT, TEXT, PING, FILE_S, FILE_E };

class Server {
public:
	char name[MAX_NAME_LEN];
	static const int size = CHR_NUM-1;
	SOCKET listenSocket, acceptSocket[size], udpSocket[size];
	WSADATA wsaData;
	bool open;
	int connected = 0;
	int slot_queue[size] = {-1};
	int f = 0; // front of the queue
	int r = 0; // rear of the queue
	FILE* files[50];
	int file_count = 0;
	Server(string sn);							// Creates the Server
	int start();								// Opens the Server for connections
	int shutdown();								// Kicks all clients and closes Server doesnt delete server
	void send_msg(char m[MAX_MSG_LEN]);			// Sends a text message to all clients
	void send_file(FILE f, int file_slot);		// Sends a file to all clients
	void kick(int slot);						// Kicks a player based on their slot
	void kick(char player[MAX_NAME_LEN]);		// Kicks a player based on their username
private:
	void sock_listen();
	void check_client(int slot, sockaddr_in c_addr);
	void recv_tcp(int slot);
	void recv_udp(int slot, sockaddr_in c_addr);
	void ping_udp(int slot, sockaddr_in c_addr);
};


class Client {
public:
	char name[MAX_NAME_LEN];
	SOCKET clientSocket, udpSocket;
	WSADATA wsaData;
	FILE* files[50];
	Client(string cn);							// Creates the client node
	int join(string IP);						// Joins a server with the given ip
	void send_msg(char m[MAX_MSG_LEN]);			// Send a text message to all clients
	void disconnect();							// Disconnect from the server
private:
	void recv_tcp();
	void recv_udp(sockaddr_in c_addr);
	void ping_udp(sockaddr_in c_addr);
	bool recvFiles(int file_slot);
};

class Buffer {
public:
	char from[MAX_NAME_LEN];
	char to[MAX_NAME_LEN];
	dataType type;
	char msg[MAX_MSG_LEN];
	int file_slot;
};

class SendBuff : public Buffer {
public:
	SendBuff(char sender[MAX_NAME_LEN], char target[MAX_NAME_LEN], dataType dType, char data[MAX_MSG_LEN] = (char[MAX_MSG_LEN])"", int f_slot = 0);
};

#endif // !NETWORK_H