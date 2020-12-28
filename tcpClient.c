#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 4444
#define h_addr h_addr_list[0]

int main(int argc, char *argv[]) {

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	struct hostent *host;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0) {
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	host = gethostbyname(argv[1]);
        if (host == NULL) {
        write(STDOUT_FILENO,"Cannot resolve hostname.\n",sizeof("Cannot resolve hostname.\n"));
        exit(1);
	}
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	//serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	bcopy((char *) host->h_addr,(char *)&serverAddr.sin_addr.s_addr,host->h_length);
	
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	while(1){
		//printf("Client: \t");
        const char msg[]="Please enter: ";
		write(STDOUT_FILENO,msg,sizeof(msg));
        int count=read(STDIN_FILENO,buffer,sizeof(buffer));
        buffer[count-1]='\0';
        send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, "exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
		memset(&buffer,0,sizeof(buffer));
		if(recv(clientSocket, buffer, 1024, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}
	return 0;
}