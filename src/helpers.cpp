/************************************************************************
 File: helpers.cpp
 Description: Helper methods
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


/**
 * Generates random number within [base... base+RAND_MAX]
 */

int generateRandomNumber(int base, int max) {
	srand(time(NULL)); //seed
	return base+ (rand() % max);
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
 Exits with EXIT_FAILURE
 ************************************************************************/

void terminate(std::string message) {
	std::cout << message << std::endl;
	std::cout << "Terminating..." << std::endl;
	exit(EXIT_FAILURE);
}


