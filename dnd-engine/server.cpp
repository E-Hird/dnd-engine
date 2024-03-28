#include "server.hpp"
using namespace std;

Server::Server(string n) {
	name = n;
	/// Initialises a listen socket and binds the IP and port
	// Init winsock
	int wsaerr;
	WORD wVersion = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersion, &wsaData);
	if (wsaerr != 0) {
		throw runtime_error("Winsock dll error: Not found");
	}
	else {
		println("Winsock Status: \{}", wsaData.szSystemStatus);
	}

	// Init the socket
	listenSocket = INVALID_SOCKET;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		println("Error at socket(): \{}", WSAGetLastError());
		WSACleanup();
		throw runtime_error("Server Failed.");
	}

	// Bind the Socket to IP and port
	sockaddr_in service;
	int port = 55555;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		println("Error at bind(): \{}", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		throw runtime_error("Server Failed.");
	}
	open = TRUE;
}

void Server::sock_listen() {
	int connected = 0;
	while (open) {
		if (connected < size) {
			if (listen(listenSocket, 1) == SOCKET_ERROR) {
				println("Error at listen(): \{}", WSAGetLastError());
				continue;
			}
			else {
				println("Listening for connections.");
			}

			acceptSocket[connected] = accept(listenSocket, NULL, NULL);
			if (acceptSocket[connected] == SOCKET_ERROR) {
				println("Error at accept(): \{}", WSAGetLastError());
				closesocket(acceptSocket[connected]);
				continue;
			}
			println("Client \{} joined", connected);
			// Add new thread in here
			connected += 1;
		}
	}
}

int Server::start() {
	thread listen_thread(&Server::sock_listen, this);
	listen_thread.detach();
	println("Connections open.");
	return 0;
}