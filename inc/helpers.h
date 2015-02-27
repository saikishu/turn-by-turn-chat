/************************************************************************
 File: helpers.h
 Description: Helper methods
 Turn based client server chat application
 More info:
 https://github.com/saikishu/turn-by-turn-chat/blob/master/README.md
 
 Author: Sai Kishore
 Copyright (c) 2015 Sai Kishore
 
 Free to use under the GPL license
 https://github.com/saikishu/turn-by-turn-chat/blob/master/LICENSE
 *************************************************************************/

#ifndef HELPERS_H_
#define HELPERS_H_

//Prototypes

int generateRandomNumber(int base, int max);
void printToConsole(std::string message, bool decorate);
void terminate(std::string message);

#endif /* HELPERS_H_ */
