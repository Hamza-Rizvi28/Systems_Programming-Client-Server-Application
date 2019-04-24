#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int total_char_count=0;

typedef struct processList{
    char processName[5];
    int process_ID;
    bool isActive;
    time_t startTime;
    time_t endTime;
    struct processList *next;
}procList;

typedef struct clientList {
    
}clients;
 
float add(char *token) {
    float sum=0;
    float curr_digit=0;

    while (token != NULL) {
        curr_digit=atoi(token);
        sum+=curr_digit;
        token=strtok(NULL," ");
    }
    return sum;
}

float subtract(char *token) {
    
    float result=0;
    float curr_digit=0;
    curr_digit=atoi(token);
    result=curr_digit;
    token=strtok(NULL," ");

    while (token != NULL) {
        curr_digit=atoi(token);
        result-=curr_digit;
        token=strtok(NULL," ");
    }
    return result;
}

float divide(char *token) {
    float result=0;
    float curr_digit=0;
    curr_digit=atof(token);
    result=curr_digit;
    token=strtok(NULL," ");

    while (token != NULL) {
        curr_digit=atoi(token);
        result/=curr_digit;
        token=strtok(NULL," ");
    }
    return result;
}

float multiply(char *token) {
    float result=1;
    float curr_digit=0;

    while (token != NULL) {
        curr_digit=atof(token);
        result*=curr_digit;
        token=strtok(NULL," ");
    }
    return result;
}

 bool run (char *str)  {

    int pid=fork();
        if (pid==-1) {
            perror("Fork failed");
        }
        else if (pid==0) {   
            char *args[]={str,NULL};
            pid=execvp(str,args);
    
            if (pid==-1) {
                perror("exec failed");
                return false;
            }
            return true;
        }
}


int main(int argc,char *argv[]){

    //char *token;
    int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = ntohs(INADDR_ANY);//inet_addr(INADDR_ANY);
	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	
    if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", atoi(argv[1]));
    
	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in Listening.\n");
	}


	while(1){
		
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
            write(STDOUT_FILENO,"ACCEPT FAILED\n",14);
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);
            char buff[1024];
            char *token;
            token=NULL;
            //first token
            while(1){
                memset(&buff,0,sizeof(buff));
				int err=recv(newSocket, &buff, 1024, 0);
                if (err==-1) {
                    perror("Recv Error!");
                    exit(EXIT_FAILURE);
                }
                char res[50];
                float result;
                token=strtok(buff," ");
                if(strcmp(buff, "exit") == 0){
                    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			        break;
		        }//RUN
                else if (strcmp(token,"run")==0) {
            
                    token=strtok(NULL," ");
                    bool result= run(token);
                    if (result==true) {

                        strcpy(res,"Run successful!");
                    }
                    else  {
                        strcpy(res,"Run unsuccessful!");
                    }
                    //int len=strlen(res);
                    //res[len]='\0';
                    //char ch[50];
                    //sprintf(res,"%d",len);
                    //write(STDOUT_FILENO,res,sizeof(res));
                    send(newSocket,res,strlen(res),0);
                }
                //ADDITION
                else if (strcmp(token,"add")==0){
                    token=strtok(NULL," ");
                    result= add(token);
                    ret = sprintf(res, "Result: %.2f \n", result);
	                //write(STDOUT_FILENO,res,ret);
                    send(newSocket,res,ret,0);
            //write(STDOUT_FILENO, res, ret);
                }
                //SUBTRACTION
                else if (strcmp(token,"sub")==0){
                    token=strtok(NULL," ");
                    result= subtract(token);
                    ret = sprintf(res, "Result: %.2f \n", result);
                    send(newSocket,res,ret,0);
	                //write(STDOUT_FILENO, res, ret);
                }
                //MULTIPLICATION
                else if (strcmp(token,"mul")==0){
                    token=strtok(NULL," ");
                    result= multiply(token);
                    ret = sprintf(res, "Result: %.2f \n", result);
                    send(newSocket,res,ret,0);
	                //write(STDOUT_FILENO, res, ret);
                }
                //DIVISION
                else if (strcmp(token,"div")==0){
                    token=strtok(NULL," ");
                    result= divide(token);
                    ret = sprintf(res, "Result: %.2f \n", result);
                    send(newSocket,res,ret,0);
	                //write(STDOUT_FILENO, res, ret);
                }
                //EXIT
                else if (strcmp(token,"exit")==0){
                    write(STDOUT_FILENO,"Successful termination!\n",sizeof("Successful termination!\n"));
                    exit(0);
                }
                //INVALID STATEMENT
                else {
                    write(STDOUT_FILENO,"Invalid command -- Exiting!\n",sizeof("Invalid command -- Exiting!\n"));
                    strcpy(res,"INVALID COMMAND!");
                    send(newSocket,res,strlen(res),0);
                    exit(1);
                }
        
			}
		}
	}
    close(newSocket);
	
    return 0;
}