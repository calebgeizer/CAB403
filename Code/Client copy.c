#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

	#define PORT 12345    /* the port client will be connecting to */

	#define MAXDATASIZE 100 /* max number of bytes we can get at once */
void Send_Array_Data(int socket_id, char *myArray) {
	int i=0;
	char statistics;  
	/*for (i = 0; i < 2; i++) {
		printf("%c\n", myArray[i]);
		statistics = htons(myArray[i]);
		//send(socket_id, &statistics, sizeof(uint16_t), 0);
	}*/



	send(socket_id, myArray, sizeof(myArray), 0);


	
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information*/

	if (argc < 2) {
		fprintf(stderr,"usage: client hostname (or zPAddress) [portNumber]\n");
		exit(1);
	}

	if (argc > 2){
		port = atoi(argv[2]);
	} else port = PORT;


	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(port);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	
	buf[numbytes] = '\0';

	printf("Received: %s",buf);

	char *simpleArray = "yes";

	Send_Array_Data(sockfd, simpleArray);

	close(sockfd);

	return 0;
}
