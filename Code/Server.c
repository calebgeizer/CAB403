/*
*  Materials downloaded from the web. See relevant web sites listed on OLT
*  Collected and modified for teaching purpose only by Jinglan Zhang, Aug. 2006
*/


#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

	#define MYPORT 12345    /* the port users will be connecting to */
	#define ARRAY_SIZE 30 

	#define BACKLOG 10     /* how many pending connections queue will hold */
	#define MAXDATASIZE 100 /* max number of bytes we can get at once */

char *Receive_Array_Int_Data(int socket_identifier, int size) {
    int number_of_bytes, i=0;
    uint16_t statistics;

    
    int sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information*/
	//

	if ((numbytes=recv(socket_identifier, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	char *results = buf;


/*
	char *results = malloc(sizeof(char)*size);
	for (i=0; i < size; i++) {
		if ((number_of_bytes=recv(socket_identifier, &statistics, sizeof(uint16_t), 0))
		         == RETURNED_ERROR) {
			perror("recv");
			exit(EXIT_FAILURE);			
		    
		}
		results[i] = ntohs(statistics);
	}
*/

	return results;
}


char* checkMessage(char* message){
	//checks what the message is to give the client a response

	return message;
}


int main(int argc, char *argv[])
{
		int sockfd, new_fd,port;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	char *results;

/*	if (argc < 1)
	{
		fprintf(stderr,"usage: server portNumber");
	}
*/
	if (argc <= 1)
	{
		port = MYPORT;
	}

	if (argc > 1)
	{
		port = atoi(argv[1]);
	}else port = MYPORT;

	/* generate the socket */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* generate the end point */
	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(port);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
		/* bzero(&(my_addr.sin_zero), 8);   ZJL*/     /* zero the rest of the struct */

	/* bind the socket to the end point */
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}

	/* start listnening */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("server starts listening ...\n");

	/* repeat: accept, send, close the connection */
	/* for every accepted connection, use a sepetate process or thread to serve it */
	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* this is the child process */

			results = Receive_Array_Int_Data(new_fd,  ARRAY_SIZE);
			printf("%s\n", results);

			char* answer = checkMessage(results);

			if (send(new_fd, answer, sizeof(answer), 0) == -1)
				perror("send");

			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
	}
}

