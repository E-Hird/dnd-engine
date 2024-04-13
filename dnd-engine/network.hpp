#ifndef NETWORK_H
#define NETWORK_H

#include <print>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread>

#define MAX_NAME_LEN 64
#define MAX_MSG_LEN 512
using namespace std;

enum dataType { CONNECT, DISCONNECT, TEXT, PING };

class Player {
public:
	char name[MAX_NAME_LEN] = "";
	int posx = 0;
	int posy = 0;
	//icon
	//ref to character sheet or something
	void init_player(char pn[MAX_NAME_LEN]);
};

class Server {
public:
	char name[MAX_NAME_LEN];
	static const int size = 9; // Number of clients allowed to join
	Player players[size];
	SOCKET listenSocket, acceptSocket[size], udpSocket[size];
	WSADATA wsaData;
	bool open;
	int connected = 0;
	int slot_queue[size] = {-1};
	int f = 0; // front of the queue
	int r = 0; // rear of the queue
	Server(string sn);
	int start();
	int shutdown();
	void send_msg(char m[MAX_MSG_LEN]);
	void kick(int slot);
	void kick(char player[MAX_NAME_LEN]);
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
	Client(string cn);
	int join(string IP);
	void send_msg(char m[MAX_MSG_LEN]);
	void disconnect();
private:
	void recv_tcp();
	void recv_udp(sockaddr_in c_addr);
	void ping_udp(sockaddr_in c_addr);
};

class Buffer {
public:
	char from[MAX_NAME_LEN];
	char to[MAX_NAME_LEN];
	dataType type;
	char msg[MAX_MSG_LEN];
	char ping[MAX_MSG_LEN];
};

class SendBuff : public Buffer {
public:
	SendBuff(char sender[MAX_NAME_LEN], char target[MAX_NAME_LEN], dataType dType, char data[MAX_MSG_LEN] = (char[MAX_MSG_LEN])"");
};


#endif // !NETWORK_H