/************************************************************************
 File: client.h
 Description: Windows client header
 Turn based client server chat application
 More info:
 https://github.com/saikishu/turn-by-turn-chat/blob/master/README.md
 
 Author: Sai Kishore
 Copyright (c) 2015 Sai Kishore
 
 Free to use under the GPL license
 https://github.com/saikishu/turn-by-turn-chat/blob/master/LICENSE
 *************************************************************************/


#ifndef CLIENT_H_
#define CLIENT_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>


#define MAXBUFFERSIZE 500 //bytes

//Prototypes
void doSend(SOCKET nSockfd);
void doReceive(SOCKET nSockfd );
void printToConsole(std::string message, bool decorate);
void terminate(std::string message);

#endif /* CLIENT_H_ */