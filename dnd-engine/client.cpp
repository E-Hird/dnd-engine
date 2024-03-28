#include "client.hpp"
using namespace std;

Client::Client(string n) {
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
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		println("Error at socket(): \{}", WSAGetLastError());
		WSACleanup();
		throw runtime_error("Server Failed.");
	}
}


int Client::join(string IP) {
	sockaddr_in clientService;
	int port = 55555;
	clientService.sin_family = AF_INET;
	wstring ip_str = wstring(IP.begin(), IP.end());
	PCWSTR ip = ip_str.c_str();
	InetPton(AF_INET, ip, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		println("Error at connect(): \{}", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	else {
		println("Connected.");
		return 0;
	}
}