#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#define BLKSIZE 1024
#define PORT 2343
#define BACKLOG 10 // how many pending connections queue will hold

int main(int argc, char *argv)
{
		system("clear");
		int tofd, bytesread, commfd, fd, b, l, totalbytes = 0;
		char buff[BLKSIZE];	
		memset(&buff,0,sizeof(buff));
	    struct sockaddr_in myinfo;
		struct sockaddr_in dest;
		socklen_t socksize = sizeof(struct sockaddr_in);		
		memset(&myinfo, 0, sizeof(myinfo));
		myinfo.sin_family = AF_INET;
		myinfo.sin_addr.s_addr = htonl(INADDR_ANY);
		myinfo.sin_port = htons(PORT);
		int sd = socket(AF_INET, SOCK_STREAM,0);
		if(sd == -1)
		{
			printf("failed to open socket\n");
			exit(0);
		}		
		b = bind(sd, (struct sockaddr *)&myinfo, sizeof(struct sockaddr)); 
		if(b == -1)
		{
			printf("failed to bind\n");
			exit(0);
		}		
		l = listen(sd,BACKLOG);
		if(l == -1)
		{
			printf("failed to listen\n");
			exit(0);
		}		
		
		tofd = open("/home/asad/project_NP_Torrent/c2.jpg", O_RDONLY);
		if(tofd == -1)
		{
			printf("failed to open s2\n");
			exit(0);
		}
				
		while(1)
		{
			printf("Waiting to incoming connection\n");
			commfd = accept(sd, (struct sockaddr *)&dest, &socksize);
			if(commfd == -1)
			{
				printf("failed to accept\n");
				exit(0);
			}		
			while(1)
			{
				bytesread =read(tofd,buff,1024 );
				if(bytesread == 0)
					break;
				int sent = send(commfd, buff, bytesread, 0);
				totalbytes += sent;			
				printf("CHUNK # 2 \t Total Bytes Sent: %d\n",totalbytes);
			}
			close(commfd);
		}
		close(tofd);
		close(sd);
		return 0;
}
 
