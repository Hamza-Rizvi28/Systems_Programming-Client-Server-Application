# Systems_Programming-Client-Server-Application

Overview:

In this project, I created a client server shell. The client requests server to run processes and can also give a list of numbers on which a specific mathematical operation can be performed like addition, subtraction, multiplication and division. The server then returns the result to the client.

The Inter-process communication is achieved through the use of sockets.

While compiling server, user needs to give a port number as a command line argument. And while compiling client, user needs to give an IP address and a port number respectively as command line arguments. Once the client program executes, it waits for the user to give an input. If the user inputs &quot;exit&quot;, the client disconnects from the server.

Example:

Client side:

compilation: gcc -o [name] tcpClient.c

Execution: ./[name] [ip\_address] [port]

port must be that of server

Server side:

compilation: gcc -o [name] tcpServer.c

Execution: ./[name] [port]

port can be any number between 2000 and 65535

Valid Commands:

- add [operand1] [operand2] [operand3]…[operandn]
- sub [operand1] [operand2] [operand3]…[operandn]
- mul [operand1] [operand2] [operand3]…[operandn]
- div [operand1] [operand2] [operand3]…[operandn]
- run [program\_name]
- Space delimeted

Architecture:

Client side:

- Forms a socket
- Connect to the server side socket
- Take user input in an infinite loop
- Send that user input to server through socket
- Client waits for the server to send the output

Server side:

- Forms a socket
- Accepts connections from clients in a loop
- After accepting a connection, forks to create a new process
- Takes their request

Limitations:

- The current project can only make connections with 5 clients.
- Also the number of processes that a single client can open at a time is equal to thirty.
- Limited process creation is due to array used for process list.
- New process entry is over riding the previous process in process list.
- Fixed input array.
- Non- multi threaded client.
