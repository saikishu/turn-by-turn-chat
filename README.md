# turn-by-turn-chat
Turn based client-server chat application using socket programming.

##Components
Linux client

Windows Client

Linux server

#Build Instructions

##Compiling Linux server and client:

- On a linux/mac machine, execute `make` in the root dir of the source code.
-  Generates `client` and `server` programs.

##Compiling Windows client:

- Download dir `windows-client` to a windows machine with Visual Studio 2012+ (earlier versions not tested)
- Open `client.sln` and Build Solution (F7) using `Release | Win 32` Configuration.
- `client.exe` will be built in `windows-client\Release` dir.

#Usage
##Step 1: Start Linux Server
- `./server`
- Note the port that is displayed.

```
./server

Starting server....
Getting an available port...
Got a free port: 62576

***************************************
Server started successfully on Port: 62576

Waiting for connections.....
***************************************

```

##Step 2: Start One of the Client application
- Linux client: `./client <hostname or ipaddr> <port>`
- Windows client: `client.exe <hostname or ipaddr> <port>`
- Parameters:

 > `hostname`: hostname of server to connect
 
 > `port` : port on which server is listening. Printed to stdout when `server` is started.
 
```
./client localhost 62576
Connecting to server....

***************************************
Successfully connected to server.
Start chatting. Press "end" when done.
***************************************

```  
- Keep chatting if you see `Your turn` message on either consoles/terminals and type `end` when done to pass the turn.

#####Example
- Client:

```
***************************************
Successfully connected to server.
Start chatting. Press "end" when done.
***************************************

**** Your turn *****
Hello server
My name is client
end
**** Receive Mode: please wait for your turn *****

```
- Server:

```
***************************************
Server started successfully on Port: 62576

Waiting for connections.....
***************************************

[Server Message] Got connection from client: 127.0.0.1
Chat started. Waiting to receive messages...
****************************************
**** Receiving.. please wait for your turn *****
Hello server
My name is client
**** Your turn *****
Hello client
My name is server
end
**** Receiving.. please wait for your turn *****
```

- Client:

```
**** Receive Mode: please wait for your turn *****
Hello client
My name is server
**** Your turn *****

```

