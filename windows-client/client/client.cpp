/************************************************************************
 File: client.h
 Description: Windows client application
 Turn based client server chat application
 More info:
 https://github.com/saikishu/turn-by-turn-chat/blob/master/README.md
 
 Author: Sai Kishore
 Copyright (c) 2015 Sai Kishore
 
 Free to use under the GPL license
 https://github.com/saikishu/turn-by-turn-chat/blob/master/LICENSE
 *************************************************************************/

#include "stdafx.h"
#include "client.h"

//Need to linking with below libraries.
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

int main(int argc, char** argv) {

	int nStatus;
	SOCKET nSockfd = INVALID_SOCKET;
	struct addrinfo hints, *serverInfo;
	struct sockaddr_in *addr;
	WSADATA wsaData;

	//Usage: linux-client hostname or ip port
	if (argc != 3) {
		terminate("Usage: client <hostname|ipaddr> <portnum>");
	}

	//Initial Configuration
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM; //streaming sockets
	printToConsole("Connecting to server....", false);

    nStatus = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (nStatus != 0) {
        printf("Initialization error: %d\n", nStatus);
        terminate("Startup error"); 
    }

	//lookup ipv4 data

    if ( (nStatus = getaddrinfo(argv[1], argv[2], &hints, &serverInfo)) != 0 ) {
        printf("Connection error: %d\n", nStatus);
      
		terminate("Connection error");
    }

	//Load up ipv4 data : not considering multiple ip's
	addr = (struct sockaddr_in *) serverInfo->ai_addr;
	
	if ((nSockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
		serverInfo->ai_protocol)) == INVALID_SOCKET) {
		printf("Socket Error: %ld\n", WSAGetLastError());
    	terminate("Error: Socket error");
	}

	if(connect(nSockfd, serverInfo->ai_addr, serverInfo->ai_addrlen ) == SOCKET_ERROR ) {
		closesocket(nSockfd);
		terminate("Error: Socket Connection error");
	}

	freeaddrinfo(serverInfo);

	//Connected if here
	printToConsole("Successfully connected to server.\nStart chatting. Press \"end\" when done.", true);
	doSend(nSockfd);

	closesocket(nSockfd);
	return 0;
}


void doSend(SOCKET nSockfd) {

	std::string message;
	printToConsole("**** Your turn *****", false);

	do {
		std::getline(std::cin, message);

		if (send(nSockfd, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
			printf("Send Error: %d\n", WSAGetLastError());
			closesocket(nSockfd);
			terminate("Error: Send error.");
		}

		if(message.compare("end") == 0) {
			break;
		}


	} while(1);

	doReceive(nSockfd);

}

void doReceive(SOCKET nSockfd ) {
	//Receive
	int bytesReceived;
	char buffer[MAXBUFFERSIZE];

	printToConsole("**** Receive Mode: please wait for your turn *****", false);

	do {
		if ((bytesReceived = recv(nSockfd, buffer, MAXBUFFERSIZE-1, 0)) == -1) {
				printf("Receive Error: %d\n", WSAGetLastError());
				terminate("Receiving error occurred!");
		}
		buffer[bytesReceived] = '\0';
		if(strcmp(buffer, "end") == 0 ) {
			break;
		}
		std::cout << buffer << std::endl;

		if(bytesReceived == 0) {
			//client connection closed.
			printToConsole("Chat ended. Remote server disconnected.\n", true);
			terminate ("Server disconnected");

		}

	} while(bytesReceived > 0);

	doSend(nSockfd);

}

void printToConsole(std::string message, bool decorate = false) {
	if(decorate) {
		std::cout << "\n***************************************" << std::endl;
	}

	std::cout << message << std::endl;

	if(decorate) {
			std::cout << "***************************************\n" << std::endl;
	}
}

/************************************************************************
 Function: terminate
 Description: Terminates the program and cleanups memory.
 Args:
 message     string      message to output before terminating
 Returns: none
 Notes:
 Controlled termination
 Cleanups memory and prevents leaks.
 Exits with EXIT_FAILURE
 ************************************************************************/

void terminate(std::string message) {
	std::cout << message << std::endl;
	std::cout << "Terminating..." << std::endl;
	WSACleanup();
	exit(EXIT_FAILURE);
}
