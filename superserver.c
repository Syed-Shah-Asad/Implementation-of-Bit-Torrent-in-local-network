#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#define PORT 2343
#define BACKLOG 10

/*	Structure that contains IP-Addresses Of Servers where different Chunks are present	*/
typedef struct{
	char s1[INET_ADDRSTRLEN];
	char s2[INET_ADDRSTRLEN];
	char s3[INET_ADDRSTRLEN];
}servers;


int main(int argc, char *argv[])
{
/*
	Command-Line Agruments Required
	1)IP-address of chunk-1
	2)IP-address of chunk-2
	3)IP-address of chunk-3
*/
	if(argc != 4)
	{
		printf("Invalid Command Line Arguments");
		exit(0);
	}
	servers ser; // declearation of struc
	memcpy(ser.s1, argv[1],INET_ADDRSTRLEN); // filling IP-address of Chunk-1
	memcpy(ser.s2, argv[2],INET_ADDRSTRLEN); // filling IP-address of Chunk-2
	memcpy(ser.s3, argv[3],INET_ADDRSTRLEN); // filling IP-address of Chunk-3
	printf("1st server: %s\n",ser.s1);
	printf("2nd server: %s\n",ser.s2);
	printf("3rd server: %s\n",ser.s3);
  	struct sockaddr_in myinfo;
	struct sockaddr_in dest;
	socklen_t socksize = sizeof(struct sockaddr_in);		
	memset(&myinfo, 0, sizeof(myinfo));
	myinfo.sin_family = AF_INET;
	myinfo.sin_addr.s_addr = htonl(INADDR_ANY); // client from any IP-Address can claim for this structure
	myinfo.sin_port = htons(PORT);
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		printf("failed to open socket\n");
		exit(0);
	}		
	int b = bind(listenfd, (struct sockaddr *)&myinfo, sizeof(struct sockaddr)); 
	if(b == -1)
	{
		printf("failed to bind\n");
		exit(0);
	}		
	int l = listen(listenfd, BACKLOG);
	if(l == -1)
	{
		printf("failed to listen\n");
		exit(0);
	}		
	printf("Waiting to incoming connection\n");
	while(1)
	{
		int commfd = accept(listenfd, (struct sockaddr *)&dest, &socksize); 
		if(commfd == -1)
		{
			printf("failed to open\n");
			exit(0);
		}
		send(commfd, &ser, sizeof(ser), 0); 
		close(commfd);								
	}
	close(listenfd);
	return 0;
}
