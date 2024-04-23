#include "network.hpp"
using namespace std;

//charcopy(target, source) to make target = source
void charcpy(char target[], char source[]) {
	if (sizeof(target) == sizeof(source)) {
		switch (sizeof(target)) {
		case MAX_NAME_LEN/8:
			for (int i = 0; i < MAX_NAME_LEN; i++) {
				target[i] = source[i];
			}
			break;
		case MAX_MSG_LEN/8:
			for (int i = 0; i < MAX_MSG_LEN; i++) {
				target[i] = source[i];
			}
			break;
		default:
			throw exception("unacceptable array length");
			break;
		}
	}
	else {
		throw exception("arrays must be same length");
	}
}
void charcpy(char target[], string source) {
	if (sizeof(target)*8 > source.length()) {
		for (int i = 0; i < source.length(); i++) {
			target[i] = source[i];
		}
		target[source.length()] = '\0';
	}
	else {
		throw exception("string must be shorter than array length");
	}
}

// Send Buffer constructor
SendBuff::SendBuff(char sender[MAX_NAME_LEN], char target[MAX_NAME_LEN], dataType dType, char data[MAX_MSG_LEN], int f_slot) {
	charcpy(from, sender);
	charcpy(to, target);
	type = dType;
	switch (type) {
	case PING:
		charcpy(msg, data);
		break;
	case TEXT:
		charcpy(msg, data);
		break;
	case FILE_S:
	case FILE_E:
		file_slot = f_slot;
		break;
	default:
		throw exception("Invalid Buffer type");
		break;
	}
}

//
void Player::init_player(char pn[MAX_NAME_LEN]) {
	charcpy(name, pn);
}

// Server Functions

Server::Server(string sn) {
	charcpy(name, sn);
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

void Server::recv_tcp(int slot) {
	while (acceptSocket[slot] != INVALID_SOCKET) {
		Buffer recvbuf;
		int byteCount = recv(acceptSocket[slot], (char*)&recvbuf, sizeof(Buffer), 0);
		if (byteCount > 0) {
			switch (recvbuf.type) {
			case CONNECT:
				// work out what to do with this
				break;
			case DISCONNECT:
				// close the sockets and save
				closesocket(acceptSocket[slot]);
				closesocket(udpSocket[slot]);
				acceptSocket[slot] = INVALID_SOCKET;
				udpSocket[slot] = INVALID_SOCKET;
				slot_queue[r] = slot;
				connected -= 1;
				r = (r + 1) % size;
				// wipe the player data
				// forward disconnection to other players
				char disconnect_msg[MAX_MSG_LEN];
				charcpy(disconnect_msg, string(recvbuf.from) + " Disconnected");
				send_msg(disconnect_msg);
				break;
			case TEXT:
				// Recieve and forward message to other players, display in chat
				for (int i=0; i < size; i++) {
					if ((acceptSocket[i] == INVALID_SOCKET) or i == slot) {
						continue;
					}
					else {
						SendBuff fwd(recvbuf.from, players[i].name, TEXT, recvbuf.msg);
						int byteCount = send(acceptSocket[i], (char*)&fwd, sizeof(SendBuff), 0);
						if (byteCount > 0) {
							continue;
						}
						else {
							// Error, handle by trying again/disconnectiong client or something
							continue;
						}
					}
				}
				println("\{}: \{}", recvbuf.from, recvbuf.msg);
				break;
			default:
				// Error
				break;
			}
		}
		else {
			// Error
			continue;
		}
	}
}

void Server::recv_udp(int slot, sockaddr_in c_addr) {
	socklen_t c_len = sizeof(c_addr);
	while (udpSocket[slot] != INVALID_SOCKET) {
		Buffer recvbuf;
		int bytesReceived = recvfrom(udpSocket[slot], (char*)&recvbuf, sizeof(Buffer), 0, (struct sockaddr*)&c_addr, &c_len);
		if (bytesReceived < 0) {
			//Error handle
		}
		else {
			if (recvbuf.type == PING) {
				// Change data of players
			}
		}
	}
}

void Server::ping_udp(int slot, sockaddr_in c_addr) {
	int c_len = sizeof(c_addr);
	while (udpSocket[slot] != INVALID_SOCKET) {
		for (int i = 0; i < size; i++) {
			if ((acceptSocket[i] == INVALID_SOCKET) or i == slot) {
				continue;
			}
			else {
				SendBuff pingbuf(name, players[i].name, PING);
				int bytesSent = sendto(udpSocket[slot], (char*)&pingbuf, sizeof(Buffer), 0, (struct sockaddr*)&c_addr, c_len);
				if (bytesSent < 0) {
					// Error handle
				}
			}
		}
	}
}

void Server::check_client(int slot, sockaddr_in c_addr) {
	struct timeval tv = {.tv_sec = 10000};
	setsockopt(acceptSocket[slot], SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
	Buffer buf;
	int byteCount = recv(acceptSocket[slot], (char*)&buf, sizeof(Buffer), 0);
	if (byteCount > 0 and buf.type == CONNECT) {
		// Send connection successful
		SendBuff success(name, buf.from, CONNECT);
		byteCount = send(acceptSocket[slot], (char*)&success, sizeof(SendBuff), 0);
		if (byteCount > 0) {
			println("\{} Connected.", buf.from);
		}
		// Make player object to track player
		players[slot].init_player(buf.from);
		// forward new connection to other players
		char connect_msg[MAX_MSG_LEN];
		charcpy(connect_msg, string(buf.from) + " Connected");
		send_msg(connect_msg);
		// Start tcp recieve
		thread tcp_thread(&Server::recv_tcp, this, slot);
		tcp_thread.detach();
		// Reset timeout to infinite
		tv.tv_sec = 0;
		setsockopt(acceptSocket[slot], SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
		// Send all resources / current connected players
		for (int i = 0; i < file_count-1; i++) {
			send_file(*files[i], i);
		}
		// Setup UDP connection
		udpSocket[slot] = INVALID_SOCKET;
		udpSocket[slot] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpSocket[slot] == INVALID_SOCKET) {
			println("Error at slot \{} udp socket(): \{}", slot, WSAGetLastError());
			//Error handle
		}
		thread udp_ping(&Server::ping_udp, this, slot, c_addr);
		udp_ping.detach();
		thread udp_recv(&Server::recv_udp, this, slot, c_addr);
		udp_recv.detach();
	}
	else {
		println("Connection failed");
		closesocket(acceptSocket[slot]);
		// Make the socket slot available
	}	
}

void Server::sock_listen() {
	for (int i = 0; i < size; i++) slot_queue[i] = i;
	while (open) {
		if (connected < size) {
			if (listen(listenSocket, 1) == SOCKET_ERROR) {
				println("Error at listen(): \{}", WSAGetLastError());
				continue;
			}
			else {
				println("Listening for connections...");
			}

			sockaddr_in client_address;
			client_address.sin_family = AF_INET;
			socklen_t c_len = sizeof(client_address);
			acceptSocket[f] = accept(listenSocket, (sockaddr*)&client_address, &c_len);
			if (acceptSocket[f] == SOCKET_ERROR) {
				println("Error at accept(): \{}", WSAGetLastError());
				closesocket(acceptSocket[f]);
				continue;
			}	
			thread check(&Server::check_client, this, f, client_address);
			check.detach();
			connected += 1;
			slot_queue[f] = -1;
			f = (f + 1) % size;
		}
	}
}

int Server::start() {
	thread listen_thread(&Server::sock_listen, this);
	listen_thread.detach();
	println("Connections open.");
	return 0;
}

int Server::shutdown() {
	println("Shutting down");
	// Kick all player
	for (int i = 0; i < size; i++) {
		if ((acceptSocket[i] == INVALID_SOCKET)) continue;
		else kick(i);
	}
	// Close listen socket
	closesocket(listenSocket);
	open = false;
	// Destroy server object
	return 0;
}

void Server::send_msg(char m[MAX_MSG_LEN]) {
	for (int i=0; i < size; i++) {
		if ((acceptSocket[i] == INVALID_SOCKET)) {
			continue;
		}
		else {
			SendBuff sendbuf(name, players[i].name, TEXT, m);
			int byteCount = send(acceptSocket[i], (char*)&sendbuf, sizeof(SendBuff), 0);
			if (byteCount > 0) {
				continue;
			}
			else {
				// Error, handle by trying again/disconnectiong client or something
				continue;
			}
		}
	}
	println("\{}: \{}", name, m);
}

void Server::send_file(FILE f, int file_slot) {
	for (int i = 0; i < size; i++) {
		if ((acceptSocket[i] == INVALID_SOCKET)) {
			continue;
		}
		else {
			char m[MAX_MSG_LEN] = "";
			SendBuff f_start(name, players[i].name, FILE_S, m, file_slot);
			SendBuff f_end(name, players[i].name, FILE_E, m, file_slot);
			TRANSMIT_FILE_BUFFERS t;
			t.Head = &f_start;
			t.HeadLength = sizeof(SendBuff);
			t.Tail = &f_end;
			t.TailLength = sizeof(SendBuff);
			bool sent = TransmitFile(acceptSocket[i], files[file_slot], 0, 0, NULL, &t, 0);
			if (sent == true) {
				Buffer check;
				int byteCount = recv(acceptSocket[i], (char*)&check, sizeof(Buffer), 0);
				if (byteCount > 0) {
					if (check.type == FILE_E) println("File sent");
				}
				continue;
			}
			else {
				// Error, handle by trying again/disconnectiong client or something
				continue;
			}
		}
	}
}

void Server::kick(int slot) {
	// send disconnect to player
	SendBuff kckbuf(name, players[slot].name, DISCONNECT);
	int byteCount = send(acceptSocket[slot], (char*)&kckbuf, sizeof(SendBuff), 0);
	if (byteCount > 0) {
		println("Kicked \{}", players[slot].name);
	}
	else {
		//Error handle
	}
}
void Server::kick(char player[MAX_NAME_LEN]) {
	// send disconnect to player
	SendBuff kckbuf(name, player, DISCONNECT);
	for (int i = 0; i < size; i++) {
		if ((acceptSocket[i] == INVALID_SOCKET)) {
			continue;
		}
		else if (string(players[i].name) == string(player)) {
			int byteCount = send(acceptSocket[i], (char*)&kckbuf, sizeof(SendBuff), 0);
			if (byteCount > 0) {
				println("Kicked \{}", player);
			}
			else {
				// Error, handle by trying again/disconnectiong client or something
				continue;
			}
		}
	}
}



// Client Functions
Client::Client(string cn) {
	charcpy(name, cn);
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
}

void Client::recv_tcp() {
	while (clientSocket != INVALID_SOCKET) {
		Buffer recvbuf;
		int byteCount = recv(clientSocket, (char*)&recvbuf, sizeof(Buffer), 0);
		bool done;
		if (byteCount > 0) {
			switch (recvbuf.type) {
			case CONNECT:
				// work out what to do with this
				break;
			case DISCONNECT:
				disconnect();
				break;
			case TEXT:
				// Recieve message, display in chat
				println("\{}: \{}", recvbuf.from, recvbuf.msg);
				break;
			case FILE_S:
				done = recvFiles(recvbuf.file_slot);
				break;
			default:
				// Error
				break;
			}
		}
		else {
			//terminate();
			continue;
		}
	}
}

void Client::recv_udp(sockaddr_in c_addr) {
	socklen_t c_len = sizeof(c_addr);
	while (udpSocket != INVALID_SOCKET) {
		Buffer recvbuf;
		int bytesReceived = recvfrom(udpSocket, (char*)&recvbuf, sizeof(Buffer), 0, (struct sockaddr*)&c_addr, &c_len);
		if (bytesReceived < 0) {
			//Error handle
		}
		else {
			if (recvbuf.type == PING) {
				// Change data of players
			}
		}
	}
}

void Client::ping_udp(sockaddr_in c_addr) {
	int c_len = sizeof(c_addr);
	while (udpSocket != INVALID_SOCKET) {
		SendBuff pingbuf(name, (char[MAX_NAME_LEN])"Server", PING);
		int bytesSent = sendto(udpSocket, (char*)&pingbuf, sizeof(Buffer), 0, (struct sockaddr*)&c_addr, c_len);
		if (bytesSent < 0) {
			// Error handle
		}
	}
}

bool Client::recvFiles(int file_slot) {
	FILE f;
	int byteCount = recv(clientSocket, (char*)&f, sizeof(FILE), 0);
	if (byteCount > 0) {
		Buffer check;
		byteCount = recv(clientSocket, (char*)&check, sizeof(Buffer), 0);
		if (byteCount > 0) {
			if (check.type == FILE_E) {
				files[file_slot] = &f;
				SendBuff res(name, check.from, FILE_E, (char[MAX_MSG_LEN])"", file_slot);
				byteCount = send(clientSocket, (char*)&res, sizeof(SendBuff), 0);
				return true;
			}
		}
	}
	return false;
}

int Client::join(string IP) {
	// Init the socket
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		println("Error at socket(): \{}", WSAGetLastError());
		WSACleanup();
		throw runtime_error("Socket Failed.");
	}

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
	}
	else {
		SendBuff request(name, (char[MAX_NAME_LEN])"Server", CONNECT);
		int byteCount = send(clientSocket, (char*)&request, sizeof(SendBuff), 0);
		if (byteCount > 0) {
			Buffer check;
			byteCount = recv(clientSocket, (char*)&check, sizeof(Buffer), 0);
			if (byteCount > 0 and check.type == CONNECT) {
				println("Connected");
				// Recieve data
				// Open recieve thread
				thread tcp_thread(&Client::recv_tcp, this);
				tcp_thread.detach();
				// Create UDP socket
				udpSocket = INVALID_SOCKET;
				udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				if (udpSocket == INVALID_SOCKET) {
					println("Error at udp socket(): \{}", WSAGetLastError());
					WSACleanup();
					throw runtime_error("Socket Failed.");
				}
				thread udp_ping(&Client::ping_udp, this, clientService);
				udp_ping.detach();
				thread udp_recv(&Client::recv_udp, this, clientService);
				udp_recv.detach();
				return 0;
			}
			else {
				println("Connection Failed");
				closesocket(clientSocket);
			}

		}
		else {
			println("Connection Failed");
			closesocket(clientSocket);
		}
	}
	return -1;
}

void Client::send_msg(char m[MAX_MSG_LEN]) {
	SendBuff sendbuf(name, (char[MAX_NAME_LEN])"all", TEXT, m);
	int byteCount = send(clientSocket, (char*)&sendbuf, sizeof(SendBuff), 0);
	if (byteCount > 0) {
		println("\{}: \{}", name, m);
	}
	else {
		// Error, handle by trying again/disconnectiong client or something
	}
}

void Client::disconnect() {
	SendBuff discbuf(name, (char[MAX_NAME_LEN])"server", DISCONNECT);
	int byteCount = send(clientSocket, (char*)&discbuf, sizeof(SendBuff), 0);
	if (byteCount > 0) {
		println("Disconnected.");
	}
	else {
		// Error, handle
	}
	closesocket(clientSocket);
	clientSocket = INVALID_SOCKET;
	// destroy client object
}