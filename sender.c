#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAXLINE 1024

int main(int argc, char * argv[])
{
    FILE *f2 = fopen("sender.txt", "w");
    if(argc <5){
		puts("Incomplete Arguments");
		exit(EXIT_FAILURE);
	}
	
    int senderPort = atoi(argv[1]);
    int receiverPort = atoi(argv[2]); //Not used
    int retransmissionTime = atof(argv[3]);
    int numPackets =  atoi(argv[4]);


	int socketDesc;
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr;
	
	struct timeval tv;
	tv.tv_sec = retransmissionTime/1000;
	tv.tv_usec = (retransmissionTime%1000)*1000;

	// Creating socket file descriptor
	if ( (socketDesc = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	if ( setsockopt(socketDesc, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0 ){
		perror("setsockopt failed");
		exit(EXIT_FAILURE);
		}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(buffer,0,MAXLINE);

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(senderPort);
	servaddr.sin_addr.s_addr = INADDR_ANY;
		
	int n, len;	
	for (int i = 1; i <= numPackets; i++)
	{
	    char Packet[]="Packet:00000";
		sprintf(Packet+7, "%d", i);

		sendto(socketDesc, (const char *)Packet, strlen(Packet),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
		printf("Sent %s\n",Packet);
        fprintf(f2,"Sent %s\n",Packet);
				
		n = recvfrom(socketDesc, (char *)buffer, MAXLINE,0, (struct sockaddr *) &servaddr,&len);
		buffer[n] = '\0';
		int ackID = atoi(buffer+16);

		if(n < 0){
            i--; 
            printf("Retransmission Timer Expired. "); 
            fprintf(f2,"Retransmission Timer Expired. "); 
            continue;
        }
		printf("Received %s. ", buffer);
        fprintf(f2,"Received %s. ", buffer);

		if(i+1!=ackID){i--; continue;}


	}
    printf("\n");
    fprintf(f2,"\n");

    char Packet[]="Packet:0";
    sendto(socketDesc, (const char *)Packet, strlen(Packet),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));

	close(socketDesc);
    fclose(f2); 
	return 0;

}
