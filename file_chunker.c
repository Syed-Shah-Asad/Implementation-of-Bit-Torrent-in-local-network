#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>


/*
	Function: calculate_file_size()
	Argument: file-descriptor of whom you wants to calculate the size
*/
int calculate_file_size(int fd)
{
	char buff[1024];
	int bytesread, totalbytes;
	while(1)
	{
		bytesread = read(fd,buff,1024);
		totalbytes += bytesread;
		if(bytesread == 0)
		{
			printf("Size Of File == \t %d Bytes\n\n",totalbytes);
			break;
		}	
	}
	return totalbytes;
}


/*
	Function: file_splitter()
	Description: It divdes a file into 3-chunks (Hard-coded)
	Argument: Size of File (Return value of function:: calculate_file_size())
*/
void file_splitter(int size)
{
	int tofd, tofd2, tofd3, fromfd, bytesread, byteswritten;
	int div = size/3; //1st chunk size
	printf("chunk-1 size: %d\n", div);
	int div2 = size - (2*div); // 2nd chunk size
	printf("chunk-2 size: %d\n", div2);
	int div3 = size - (2*div2) +1; // 3rd chunk size
	printf("chunk-3 size: %d\n", div3);
	int size_check = (div+div2+div3); //total size = sum(chunk1, chunk2, chunk3)
	printf("Total size : %d\n\n", size_check);

	fromfd = open("/home/asad/torrent/lums.jpg", O_RDONLY); // opening source file
	if(fromfd == -1)
	{
		printf("failed to open source file\n");
		exit(0);
	}
	tofd = open("/home/asad/torrent/s1.jpg", O_WRONLY | O_TRUNC); // opening a file where chunk-1 has to be written
	if(tofd == -1)
	{
		printf("failed to open s1\n");
		exit(0);
	}
	tofd2 = open("/home/asad/torrent/s2.jpg", O_WRONLY | O_TRUNC); // opening a file where chunk-2 has to be written
	if(tofd2 == -1)
	{
		printf("failed to open s2\n");
		exit(0);
	}
	tofd3 = open("/home/asad/torrent/s3.jpg", O_WRONLY | O_TRUNC); // opening a file where chunk-3 has to be written
	if(tofd3 == -1)
	{
		printf("failed to open s2\n");
		exit(0);
	}

	int totalbytes = 0;
	char buff[1024];
	while(1)
	{
		bytesread = read(fromfd, buff, 1024);
		totalbytes += bytesread;
		printf("Bytes read = %d\n", totalbytes);
		if(bytesread == 0)
			break;
		if(totalbytes <= div) // Write in chunk-1 untill its limit is defined
		{
			byteswritten = write(tofd, buff, bytesread);
			puts("CHUNCK # 1");
			printf("bytes written = %d\n",byteswritten);
		}
		else if(totalbytes <= (div+div2)) // Write in chunk-2 untill its limit is defined
		{
			byteswritten = write(tofd2, buff, bytesread);
			puts("CHUNCK # 2");
			printf("bytes written = %d\n",byteswritten);
		}	
		else if(totalbytes <= (div+div2+div3)) // Write in chunk-3 untill its limit is defined
		{
			byteswritten = write(tofd3, buff, bytesread);
			puts("CHUNCK # 3");
			printf("bytes written = %d\n",byteswritten);
		}	
	}
	close(fromfd);
	close(tofd);
	close(tofd2);
	close(tofd3);
}


/*
	Function: files_joiner()
	Description: This function is just to test whether we get the same file again if we join all the chunks together.
*/
void file_joiner()
{
	int tofd, tofd2, tofd3, fromfd, bytesread, byteswritten;
	fromfd = open("/home/asad/torrent/final.jpg", O_WRONLY); //opening destination file
	if(fromfd == -1)
	{
		printf("failed to open source file\n");
		exit(0);
	}
	tofd = open("/home/asad/torrent/s1.jpg", O_RDONLY); //opening chunk-1
	if(tofd == -1)
	{
		printf("failed to open s1\n");
		exit(0);
	}
	tofd2 = open("/home/asad/torrent/s2.jpg", O_RDONLY); //opening-chunk-2
	if(tofd2 == -1)
	{
		printf("failed to open s2\n");
		exit(0);
	}
	tofd3 = open("/home/asad/torrent/s3.jpg", O_RDONLY); //opening chunk-3
	if(tofd2 == -1)
	{
		printf("failed to open s2\n");
		exit(0);
	}
	char buff[1024];
	int totalbytes = 0;
	while(1)
	{
		//Reading from chunk-1 & writing in destination file
		bytesread = read(tofd,buff,1024 );
		totalbytes += bytesread;
		printf("Bytes read = %d\n", totalbytes);
		if(bytesread == 0)
			break;
		byteswritten = write(fromfd, buff, bytesread);
		puts("CHUNK #1 processing . . ");
	}
	while(1)
	{
		//Reading from chunk-2 & writing in destination file
		bytesread =read(tofd2,buff,1024 );
		totalbytes += bytesread;
		printf("Bytes read = %d\n", totalbytes);
		if(bytesread == 0)
			break;
		byteswritten = write(fromfd, buff, bytesread);
		puts("CHUNK #2 processing . . ");		
	}
	while(1)
	{
		//Reading from chunk-2 & writing in destination file
		bytesread =read(tofd3,buff,1024);
		totalbytes += bytesread;
		printf("Bytes read = %d\n", totalbytes);
		if(bytesread == 0)
			break;
		byteswritten = write(fromfd, buff, bytesread);
		puts("CHUNK #3 processing . . ");		
	}
	close(fromfd);
	close(tofd);
	close(tofd2);
	close(tofd3);	
}


/*
	Main Function From Where Execution Of Program Begins!
*/
int main(int argc, char *argv[])
{
	system("clear");
	int tofd1, tofd2, tofd3, fromfd;
	fromfd = open("/home/asad/torrent/lums.jpg", O_RDONLY); //opening source file
	if(fromfd == -1)
	{
		printf("failed to open source file\n");
		exit(0);
	}
	int size = calculate_file_size(fromfd); //calculating size of source file
	close(fromfd); //closing source file
	file_splitter(size); //making chunks
	file_joiner(); //testing whether it works after combining all the chuks
	return;
}
