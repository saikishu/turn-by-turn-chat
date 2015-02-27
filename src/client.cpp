/************************************************************************
 File: client.cpp
 Description: Linux chat client
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
#include "../inc/client.h"

/************************************************************************
 Function: main
 Description: Main entry point to start client
 usage: client 	<hostname or ipaddress> <port>
 Args:
 argc	  int		argument count
 argv     char*      pointer to commandline arguments
 Returns: 0 on successful termination.
 Notes:
 IPv4 only
 Terminates on
 bad arguments.
 socket errors.
 connection errors.
 connection closed by server.
 ctrl/cmd + c to terminate.
 ************************************************************************/

int main(int argc, char* argv[]) {

	int nStatus, nSockfd, nBytes;
	char buffer[MAXBUFFERSIZE];
	struct addrinfo hints, *serverInfo;
	struct sockaddr_in *addr;

	if (argc != 3) {
		terminate("Usage: client <hostname|ipaddr> <portnum>");
	}

	//Initial Configuration
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM; //streaming sockets

	printToConsole("Connecting to server....", false);

	if ((nStatus = getaddrinfo(argv[1], argv[2], &hints, &serverInfo)) != 0) {

		perror("Connection Error:");
		terminate(
				"Error: Please check if server is up and running. Connection failed: "
						+ std::string(gai_strerror(nStatus)));
	}

	//Load up ipv4 data : not considering multiple ip's
	addr = (struct sockaddr_in *) serverInfo->ai_addr;

	if ((nSockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
			serverInfo->ai_protocol)) == -1) {
		perror("Socket Error:");
		terminate("Error: Socket error");
	}

	if (connect(nSockfd, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
		perror("Socket Error:");
		terminate("Error: Socket error");
	}

	freeaddrinfo(serverInfo);

	//Connected if here

	printToConsole(
			"Successfully connected to server.\nStart chatting. Press \"end\" when done.",
			true);

	//start chat
	doSend(nSockfd);

	return 0;
}

/************************************************************************
 Function: doSend
 Description: Sets up client in send mode.
 Args:
 nSockfd     int      socket file descriptor
 Returns: none
 Notes: Terminates on sending errors.
 ************************************************************************/

void doSend(int nSockfd) {

	std::string message;
	printToConsole("**** Your turn *****", false);

	do {
		std::getline(std::cin, message);

		if (send(nSockfd, message.c_str(), message.length(), 0) == -1) {
			perror("Send Error:");
			terminate(
					"Error: Send error. Please check if server is up and listening.");
		}

		if (message.compare("end") == 0) {
			break;
		}

	} while (1);

	doReceive(nSockfd);

}

/************************************************************************
 Function: doReceive
 Description: Sets up client in receive mode.
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

	printToConsole("**** Receive Mode: please wait for your turn *****", false);

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
			printToConsole("Chat ended. Remote server disconnected.\n", true);
			terminate("Server disconnected");

		}

	} while (bytesReceived > 0);

	doSend(nClientfd);

}
