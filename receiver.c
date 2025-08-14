#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 1024

int main(int argc, char * argv[])
{
    FILE *f1 = fopen("receiver.txt", "w");
	if(argc <4){
		puts("Incomplete Arguments");
		exit(EXIT_FAILURE);
	}
	
    int receiverPort = atoi(argv[1]);
    int senderPort = atoi(argv[2]);  //Not used
    double packetDropProbability = atof(argv[3]);

	int socketDesc;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr, cliaddr;
	double randNum;
	srand ( time ( NULL));	

	// Creating socket file descriptor
	if ( (socketDesc = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(receiverPort);
		
	// Bind the socket with the server address
	if ( bind(socketDesc, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	int len, n;
	int expectedID = 1;
	for (;;)
	{		
	
		len = sizeof(cliaddr); //len is value/result
		
		n = recvfrom(socketDesc, (char *)buffer, MAXLINE,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr,
					&len);
		buffer[n] = '\0';
		
        int packetID = atoi(buffer+7);
        
        if(packetID==0){ // Terminating sequence
            printf("Terminating Sequence Received.\n");
            fprintf(f1,"Terminating Sequence Received.\n");
            break;
        }
        printf("Received %s. ", buffer);
        fprintf(f1,"Received %s. ", buffer);

		if(packetID==expectedID){
        
			randNum = (double)rand()/RAND_MAX; 

			if(randNum>=packetDropProbability){
				expectedID++;
				char ACK[]="Acknowledgement:00000";
				sprintf(ACK+16, "%d", expectedID);
               

				sendto(socketDesc, (const char *)ACK, strlen(ACK),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				printf("Sent %s\n",ACK);
                fprintf(f1,"Sent %s\n",ACK);
			
			} else {
                printf("Dropped %s\n",buffer); 
                fprintf(f1,"Dropped %s\n",buffer); 
            }
			
		} else {
			char ACK[]="Acknowledgement:000000";
			sprintf(ACK+16, "%d", expectedID);
            

			sendto(socketDesc, (const char *)ACK, strlen(ACK),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
			printf("Sent %s\n",ACK);
            fprintf(f1,"Sent %s\n",ACK);
		}
	}
    fclose(f1); 
	return 0;

     
}
