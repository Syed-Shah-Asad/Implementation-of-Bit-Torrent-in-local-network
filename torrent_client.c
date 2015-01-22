#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BLKSIZE 1024
#define PORTNUM 2343

/*	Struction that contains IP-Addresses Of Servers where different Chunks are present	*/
typedef struct{
	char s1[INET_ADDRSTRLEN];
	char s2[INET_ADDRSTRLEN];
	char s3[INET_ADDRSTRLEN];
}servers;


servers ser; //global declaration of structure that contails IP-addresses
int totalbytes; //global declataion of total bytes rece

/* 
	Function to request from Super-Server to get the track (IP-Addresses) of all server where different chunks are present!! 
	Arugument: IP address of Super-Server.

*/
void get_servers(char ip[]) 
{
	int mysocket;
	struct sockaddr_in dest; 
	mysocket = socket(AF_INET, SOCK_STREAM, 0); 
	memset(&dest, 0, sizeof(dest));               
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(ip); /* set destination IP number */ 
	dest.sin_port = htons(PORTNUM);               
	connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr));
	while(1)
	{
		int bytes = recv(mysocket, &ser, sizeof(ser), 0); /* receive the structure that contains the IP-addresses*/
		if(bytes <= 0)
		{
			break; // nothing is left to recv, come out of the loop!
		}
		if(bytes != sizeof(ser))
		{
			continue; //Ensure whether client have received all the IP-address? as superserver sending continuously the structure!!
		}
	}
	close(mysocket);
}
 
/*************************************************************************************************************************/

/*
	Function to request for chunks.
	Argument-1: file descriptor opened in write-mode where 
	Argument-2: IP-address where your chunk is present
*/ 
void request_file(int fd, char ip[])
{
		int mysocket;
		mysocket = socket(AF_INET, SOCK_STREAM, 0);
		if(mysocket == -1)
		{
		   printf("failed to open socket\n");
		   exit(0);
		}
		char buff[BLKSIZE];
		int BytesRecv, fromfd, byteswritten, commfd = 0;
		struct sockaddr_in dest; 
		memset(&dest, 0, sizeof(dest));                /* zero the struct */
		dest.sin_family = AF_INET;
		dest.sin_addr.s_addr = inet_addr(ip); /* set destination IP number */ 
		dest.sin_port = htons(PORTNUM);                /* set destination port number */
		printf("connecting . . . \n");
		commfd = connect(mysocket, (struct sockaddr *)&dest, sizeof(dest));
		if(commfd == -1)
		{
		   printf("failed to connect\n");
		   exit(0);
		}		
		while(1)
		{
			BytesRecv = recv(mysocket, buff, sizeof(buff), 0);
			if(BytesRecv == 0)
			{
				break; //if nothing is received, come out of the loop
			}
			byteswritten = write(fd, buff, BytesRecv);
			totalbytes += byteswritten; //calculating how many bytes have been received & written!!
			printf("Bytes Written = %d\n", totalbytes);
		}
		close(commfd);		
		close(mysocket);
}

/****************************************************************************************************************/



	/* Main Function */
int main(int argc, char *argv[])
{
	system("clear");
	puts("Welcome To Bit_Torrent");
	if(argc != 2)
	{
		printf("Invalid Command Line Argument: Super-Server's IP missing\n");
		exit(0);
	}
	int fromfd = open("/home/dcse-lab1/torrent/try/new.jpg", O_WRONLY);
	if(fromfd == -1)
	{
		printf("Failed to Open Destination File..!\n");
		exit(0);
	}
	
	get_servers(argv[1]); //Function Call to get the track to IP-addresses!
	
	printf("IP Track: %s -> %s -> %s\n", ser.s1, ser.s2, ser.s3); 

	request_file(fromfd, ser.s1); // requesting servers for the chunks!
	request_file(fromfd, ser.s2);
	
	puts("\t\t\tDownloading Completed");
	close(fromfd);
 	return EXIT_SUCCESS;
}
