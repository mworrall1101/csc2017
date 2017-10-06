#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define SEND_BUFF_SIZE 100
#define RECV_BUFF_SIZE 1024
#define SERV_PORT_NO 8080
#define SERV_IP_ADDR  "127.0.0.1"
#define FILE_NAME "sample_file.txt"
int main(int argc, char* argv[])
{
    int sockfd = 0;
    int  bytesReceived = RECV_BUFF_SIZE;
    int bytesSent = 0;
    ssize_t recsize;
    char sendBuff[SEND_BUFF_SIZE];
    char recvBuff[RECV_BUFF_SIZE];

    if(argc!=2) {
        printf("Usage: ./ufc [filename.txt]\n");
        exit(0);
    }

    //memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in sa;
    socklen_t length;
    /* Create a socket first */
    if((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Zero out socket address */
    memset(&sa, 0, sizeof sa);
    /* Initialize sockaddr_in data structure */
    sa.sin_family = AF_INET;
    sa.sin_port = htons(SERV_PORT_NO); // port
    sa.sin_addr.s_addr = inet_addr(SERV_IP_ADDR);
    length = sizeof sa;

    strcpy(sendBuff, argv[1]);  

    /*Send the filename*/
    bytesSent = sendto(sockfd, sendBuff, strlen(sendBuff), 0,(struct sockaddr*)&sa, sizeof sa);
    if (bytesSent < 0) {
        printf("Sendto: Error sending the file name: %s\n", strerror(errno));
        return 1;
      }    

    printf("Filename sent!\n");    

    /* Create file where data will be stored */
    FILE *fp;
    fp = fopen("received.txt", "w"); 
    if(NULL == fp)
    {
        printf("Error opening the file");
        return 1;
    }

    printf("Begin receiving the file...\n"); 
    while(bytesReceived==RECV_BUFF_SIZE) {
        /* Receive data in chunks of 1024  bytes */
        memset(recvBuff, 0, sizeof(recvBuff));

        bytesReceived = recvfrom(sockfd, recvBuff, sizeof(recvBuff), 0, (struct sockaddr *)&sa, &length);
        /*Check for errors*/
        if (bytesReceived<0) {
           printf("Recvfrom: Error in recieving the file\n");
           exit(1);

        } else if(bytesReceived==0){
            printf("Transmission ended.\n");
            fclose(fp);
            close(sockfd);
            return 0;
        } else {   
          printf("Number of bytes received: %d\n", bytesReceived);
          //printf("%s\n", recvBuff);
         }

        if(fwrite(recvBuff,1,bytesReceived,fp)<0)
         {
           printf("Error writting the file\n");
           exit(1);
         }
    }
    
   fclose(fp);
   close(sockfd);
   return 0;
}
