/************************************************************************
 File: server.h
 Description: Server Header
 Turn based client server chat application
 More info:
 https://github.com/saikishu/turn-by-turn-chat/blob/master/README.md
 
 Author: Sai Kishore
 Copyright (c) 2015 Sai Kishore
 
 Free to use under the GPL license
 https://github.com/saikishu/turn-by-turn-chat/blob/master/LICENSE
 *************************************************************************/

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define QUEUE_SIZE 15 //pending connections
#define MAXBUFFERSIZE 500 //bytes

//Prototypes
int bindAndGetAvailablePort();
int getSocket();
void doSend(int nSockfd);
void doReceive(int nClientfd);

#endif /* SERVER_H_ */
