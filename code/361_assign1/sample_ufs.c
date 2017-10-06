#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RECV_BUFF_SIZE 100
#define SEND_BUFF_SIZE 1024
#define PORT_NO 8080

int main(int argc, char* argv[]) {

	if(argc!=2) {
		printf("Usage: ./ufs [directory]\n");
		return 0;
	}
	int max_pathlen = 100;
	char dir[50] = "";
	char fpath[max_pathlen];
	int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	struct sockaddr_in sa; 
	char recvBuff[RECV_BUFF_SIZE];
	ssize_t recsize;
	socklen_t fromlen;
	char sendBuff[SEND_BUFF_SIZE];
	int numrv;
	struct stat file_info;

	/*Check directory argument and ensure:
			-it exist and is a directory
			-the trailing char is a '/', otherwise concat a '/'
	*/
	if (stat(argv[1], &file_info)==-1) {
		printf("Error selecting directory: %s\n", strerror(errno));
	} else if (S_ISDIR(file_info.st_mode)) {
		strcpy(dir, argv[1]); 
	}
	
	char* str_index = rindex(dir, (int)'/');
	if(*(++str_index) !='\0') {
		strcat(dir, "/");
	}

	printf("Current working directory is: %s\n", dir);


	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(PORT_NO);
	fromlen = sizeof(sa);

	if (-1 == bind(sock, (struct sockaddr *)&sa, sizeof sa)) {
		perror("error bind failed");
		close(sock);
		return 1;
	}


	for (;;) {
	  	memset(sendBuff, 0, SEND_BUFF_SIZE);
	  	memset(fpath, 0, max_pathlen);
	  	memset(recvBuff, 0, RECV_BUFF_SIZE);


		recsize = recvfrom(sock, (void*)recvBuff, RECV_BUFF_SIZE, 0, (struct sockaddr*)&sa, &fromlen);
		if (recsize < 0) {
			fprintf(stderr, "%s\n", strerror(errno));
			return 1;
		}
		printf("Requested filename: %.*s\n", (int)recsize, recvBuff);

		/* See if file is available, if not send error message to send buffer*/
		strcpy(fpath, dir);
		strcat(fpath, recvBuff);
		if(stat(fpath, &file_info)==-1) {
			strcpy(sendBuff, "Not Found\n");
			int n = sendto(sock,sendBuff, strlen(sendBuff), 0,(struct sockaddr*)&sa, sizeof sa);
			if(n<0) {
				perror("sendto: not found send failed\n");
				exit(1);
			}
			printf("Error: %s\n", strerror(errno));


		} else {

			/* Open the file that we wish to transfer */
			FILE *fp = fopen(fpath,"r");
			if(fp==NULL) {
			    printf("File open error");
			    return 1;
			}

			while(1) {
				/*Reset the send buffer*/
				memset(sendBuff, 0, sizeof(sendBuff));
				/* Read file in chunks of 1024 bytes */				
				int nread = fread(sendBuff,1,SEND_BUFF_SIZE,fp);
				printf("Bytes read %d \n", nread);
				/* If read was success, send data. */
				
				printf("Sending the file ...\n");
				int n = sendto(sock,sendBuff, strlen(sendBuff), 0,(struct sockaddr*)&sa, sizeof sa);				

				if(n<0) {
					perror("Problem sendto\n");
					exit(1);
				} else {
					if (nread < SEND_BUFF_SIZE) {
					    if (feof(fp)) {
					        printf("End of file\n");
					    	sendto(sock,sendBuff, 0, 0,(struct sockaddr*)&sa, sizeof sa);
					    }
					    if (ferror(fp))
					        printf("Error reading the file at server program\n");
					    break;
					}				       
				}
				usleep(50);
				

				
			}
		}
	}



	close(sock);
	return 0;

}
