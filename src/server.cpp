/************************************************************************
 File: server.cpp
 Description: Chat Server
 Turn based client server chat application
 More info:
 https://github.com/saikishu/turn-by-turn-chat/blob/master/README.md
 
 Author: Sai Kishore
 Copyright (c) 2015 Sai Kishore
 
 Free to use under the GPL license
 https://github.com/saikishu/turn-by-turn-chat/blob/master/LICENSE
 *************************************************************************/

#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/server.h"

/************************************************************************
 Function: main
 Description:
 Main entry point to start server.
 Starts server and listens on a port.
 usage: server
 Args:	none 	command line args ignored.
 Returns: 0 on successful termination.
 Notes:
 Ignores arguments
 IPv4 only
 Terminates on
 socket errors.
 connection errors.
 Gracefully handles client disconnections and waits for more connections.
 Max 15 connections in queue.
 ctrl/cmd + c to terminate.
 ************************************************************************/

int main(int argc, char* argv[]) {

	int nStatus, nSockfd, nClientfd;

	struct addrinfo hints, *serverInfo;
	struct sockaddr_in *addr;
	struct sockaddr_in clientAddr;

	socklen_t sinSize;
	char szIP[INET_ADDRSTRLEN];

	if (argc != 1) {
		printToConsole("Ignoring additional arguments..", false);
	}

	printToConsole("Starting server....", false);

	//Initial Configuration
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM; //streaming sockets
	hints.ai_flags = AI_PASSIVE; //localhost, let socket api take care of filling data.

	printToConsole("Getting an available port...", false);
	int port = bindAndGetAvailablePort();

	if (port < 0) {
		terminate(
				"Error getting a free port. Error code: "
						+ std::to_string(static_cast<long long>(port))); //static cast to support c++0x
	}

	printToConsole(
			"Got a free port: " + std::to_string(static_cast<long long>(port)),
			false);

	if ((nStatus = getaddrinfo(NULL,
			std::to_string(static_cast<long long>(port)).c_str(), &hints,
			&serverInfo)) != 0) {
		perror("Connection Error:");
		terminate(
				"Error: Initial configuration failed: "
						+ std::string(gai_strerror(nStatus))); //static cast to support c++0x
	}

	//Load up ipv4 data : not considering multiple ip's
	addr = (struct sockaddr_in *) serverInfo->ai_addr;
	if ((nSockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol)) == -1) {
		perror("Socket Error:");
		terminate("Socket error: Failed to open socket.");
	}

	int optval = 1;
	if (setsockopt(nSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int))
			== -1) {
		perror("Socket Error:");
		terminate("Socket Options error: Fail to set socket options.");
	}

	if (bind(nSockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror("Socket Error:");
		terminate("Socket Bind error: Failed to bind to socket. ");
	}

	uint16_t portNum = ntohs(addr->sin_port);
	inet_ntop(serverInfo->ai_family, &addr->sin_addr, szIP, sizeof szIP);
	freeaddrinfo(serverInfo);

	//Listen

	if (listen(nSockfd, QUEUE_SIZE) == -1) {
		perror("Listen Error:");
		terminate(
				"Listen error: Failed to listen"
						+ std::string(gai_strerror(errno)));
	}

	//Accept and Process

	printToConsole(
			"Server started successfully on Port: "
					+ std::to_string(static_cast<long long>(portNum))
					+ "\n\nWaiting for connections.....", true);

	while (1) {

		sinSize = sizeof clientAddr;
		nClientfd = accept(nSockfd, (struct sockaddr *) &clientAddr, &sinSize);

		if (nClientfd == -1) {
			printToConsole(
					"Error accepting client connection..continuing wait..",
					false);
			continue;
		}

		inet_ntop(clientAddr.sin_family, &(clientAddr.sin_addr), szIP,
				sizeof szIP);

		printToConsole(
				"[Server Message] Got connection from client: "
						+ std::string(szIP), false);

		printToConsole("Chat started. Waiting to receive messages...", false);
		printToConsole("****************************************", false);

		doReceive(nClientfd);

	}

	return 0;
}

/************************************************************************
 Function: bindAndGetAvailablePort
 Description: Scan for available ports and binds to an available port.
 Args: none
 Returns:
 port	int 	available port to which server can bind to.
 Notes:
 Scans maximum 500 random ports to avoid resource wastage.
 Terminates on
 binding errors.
 ports full (after 500 random scans)
 Todo:
 Potential race condition exists in acquiring ports.
 ************************************************************************/

int bindAndGetAvailablePort() {

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(inet_addr("0.0.0.0"));

	//loop as long as we get a port
	int loopCounter = 0;

	while (1) {

		//Get a random port between : 1025-65535
		int port = generateRandomNumber(1025, 65535);

		/*Get a socket*/
		int sock = getSocket();
		if (sock == -1) {
			if (loopCounter > 500) {
				//System resource wastage. So fail quick.
				return -1;
			}
		}

		/*Is port available ?*/
		addr.sin_port = htons(port);

		if (bind(sock, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
			if (errno == EADDRINUSE) {
				//Port not available so loop and check for another
				continue;
			} else {
				//Any Other Bind error
				return -2;
			}
		}

		socklen_t length = sizeof addr;
		if (getsockname(sock, (struct sockaddr*) &addr, &length) != 0) {
			return -3;
		} else {
			break;
		}
		loopCounter++;

	}
	return addr.sin_port;
}

/************************************************************************
 Function: getSocket
 Description: gets a socket
 Args: none
 Returns:
 socket	int 	scoket file descriptor.
 Notes:
 Returns -1 on socket error.
 ************************************************************************/

int getSocket() {
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	return sock;
}

/************************************************************************
 Function: doReceive
 Description: Sets up server in receive mode.
 Args:
 nClientfd     int      client socket file descriptor
 Returns: none
 Notes:
 Terminates on receiving errors.
 Gracefully handles disconnections.
 ************************************************************************/

void doReceive(int nClientfd) {
	//Receive
	int bytesReceived;
	char buffer[MAXBUFFERSIZE];

	printToConsole("**** Receiving.. please wait for your turn *****", false);

	do {
		if ((bytesReceived = recv(nClientfd, buffer, MAXBUFFERSIZE - 1, 0))
				== -1) {
			perror("Receive Error:");
			terminate("Receiving error occurred!");
		}
		buffer[bytesReceived] = '\0';

		if (strcmp(buffer, "end") == 0) {
			break;
		}
		std::cout << buffer << std::endl;

		if (bytesReceived == 0) {
			//client connection closed.
			printToConsole(
					"Chat ended. Remote client disconnected.\n Waiting for more connections...",
					true);

		}

	} while (bytesReceived > 0);

	doSend(nClientfd);

}

/************************************************************************
 Function: doSend
 Description: Sets up server in send mode.
 Args:
 nSockfd     int      socket file descriptor
 Returns: none
 Notes:
 Terminates on sending errors.
 ************************************************************************/

void doSend(int nSockfd) {

	std::string message;
	printToConsole("**** Your turn *****", false);

	do {
		std::getline(std::cin, message);
		if (send(nSockfd, message.c_str(), message.length(), 0) == -1) {
			perror("Send Error:");
			terminate("Error: Send error.");
		}
		if (message.compare("end") == 0) {
			break;
		}

	} while (1);

	doReceive(nSockfd);

}
