#include <iostream>
#include <fstream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <signal.h>
#include <string>
#define MAX 80
#define PORT 4005
#define thread_max 4
#define SA struct sockaddr
std::fstream fileptr;
struct msg{
  char filename[200];
  char word[20];
  int value;
};

int sockfd;
int count;
std::string key;

int current_thread = 0;
std::string file[1000];

void sig_handler(int signo)
{
  if (signo == SIGINT)
  {
		std::cout<<"Server is shutting down...";
		close(sockfd);
		exit(0);
	}
}

void *func(void *arg)
{
  int sockfd = *((int *)arg);
  struct msg buff;
std::cout.flush();

  for(;;){
    std::fstream fileptr;
		bzero(buff.filename, sizeof(buff.filename));
    bzero(buff.word, sizeof(buff.word));

		read(sockfd, &buff, sizeof(buff));

    std::cout<<"From Client Filename "<<buff.filename<<"\n";
    std::cout<<"From client Word " <<buff.word<<"\n";
    fileptr.open(buff.filename);
    key = buff.word;
    std::cout<<"key"<<"\n";
    std::string text;
    fileptr.seekg(0);
    while(getline(fileptr, text))
    {
      count++;
    }
    std::cout<< "Number of lines "<<count<<"\n";
    //for(int i=0; i<sizeof(buff.word) ; i++)
    //{
    //    buff.word[i]=i+1;
    //}
    //std::cout<<"hey";
    std::string textword;
    while(fileptr>>textword)
    {
      if(key == textword)
      {
        buff.value = 1;
        write(sockfd, &buff, sizeof(buff));
      }
    }
    bzero(buff.filename, sizeof(buff.filename));
    bzero(buff.word, sizeof(buff.word));

    std::cout.flush();


    //bzero(buff.value, sizeof(buff.value));

  }


}


int main()
{
	int  confd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 15)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");

    pthread_t id[40];
  int i=0;
    while(1)
    {
      if (signal(SIGINT, sig_handler) == SIG_ERR)
          printf("\ncan't catch SIGINT\n");
      len = sizeof(cli);
      confd = accept(sockfd,(SA*)&cli,(socklen_t*)&len);
      char* str;
      printf("%s",&cli.sin_addr);
      printf("\nREQUEST FROM %s PORT %d\n",inet_ntoa(cli.sin_addr),htons(cli.sin_port));

      if(confd < 0)
        std::cout<<"Server accept failed....\n";
      else
        std::cout<<"Server accept the client....\n";
      std::cout<<"Connection accepted!!!"<<"\n";


      if(pthread_create(&id[i++],NULL,func,&confd) != 0)
        std::cout<<"Failed to create thread..."<<"\n";
      else
        std::cout<<"Thread created ...\n";
    }




}
