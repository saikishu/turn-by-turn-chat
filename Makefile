all:	src/server.cpp inc/common.h inc/server.h src/helpers.cpp inc/helpers.h src/client.cpp inc/client.h 
	g++ -std=c++0x  -o server src/helpers.cpp src/server.cpp
	g++ -std=c++0x  -o client src/helpers.cpp src/client.cpp
	
debug:	src/server.cpp inc/common.h inc/server.h src/helpers.cpp inc/helpers.h src/client.cpp inc/client.h 
	g++ -std=c++0x  -g -o P2 src/server.cpp src/helpers.cpp
	g++ -std=c++0x  -o client src/helpers.cpp src/client.cpp
clean:
	rm -f *.o *~ server client core