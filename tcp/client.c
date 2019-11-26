#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX 80
#define PORT 4005
#define SA struct sockaddr

struct msg {
	char filename[200];
	char word[20];
	int value;
	//int arr[100];
};
void func(int sockfd)
{
	struct msg buffer;
	int n;
	for (;;) {
		bzero(buffer.filename,sizeof(buffer.filename));
		bzero(buffer.word,sizeof(buffer.word));
		printf("Enter the file name : ");
		scanf("%s",buffer.filename);
		printf("Enter the word: ");
		scanf("%s",buffer.word);
		write(sockfd,&buffer, sizeof(buffer));
		bzero(buffer.filename,sizeof(buffer.filename));
		bzero(buffer.word,sizeof(buffer.word));
		read(sockfd,&buffer, sizeof(buffer));
		printf("From server :\n");
		//for(int i=0;i<sizeof(buffer.filename);i++)
		//	printf("%c\t",buffer.filename[i]);
		//printf("\n");
		//bzero(buffer.filename,sizeof(buffer.filename));

		if(buffer.value == 1)
		{
			printf("Matched");
		}
	}
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
	func(sockfd);
	close(sockfd);
}
