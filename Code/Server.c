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


	#define MAXDATASIZE 500
	#define secondArray 2

	#define MYPORT 12345    /* the port users will be connecting to */
	#define ARRAY_SIZE 30 

	#define BACKLOG 10     /* how many pending connections queue will hold */

char* checkMessage(char* message){
	//checks what the message is to give the client a response

	// WRITE CODE HERE
	int user = 0;
	user = authUser(message);

	printf("%d\n", user);

	if (user == 1)
	{
		return "success";
	}
	if (user == 0)
	{
		return "failed";
	}
	hangman();

	return message;
}




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



char* twoWords(char *hangmanWords[MAXDATASIZE][secondArray],int size){

	int i, stime;
	long ltime;

	/* get the current calendar time */
	ltime = time(NULL);
	stime = (unsigned) ltime/2;
	srand(stime);

	//select random word
	int x = rand() % size;
	int y = rand() % size;

	//printf("%d %d\n", x,y);

	char *word1 = hangmanWords[x][0];
	char *word2 = hangmanWords[y][0];
	//printf("%s %s\n", word1, word2);

	char* finalText;
	finalText = malloc(strlen(word1)+1+4); /* make space for the new string (should check the return value ...) */
	strcpy(finalText, word1); /* copy name into the new var */
	strcat(finalText, " ");
	strcat(finalText, word2); /* add the extension */

	//printf("%s\n", finalText);
	return finalText;
}


int grabFile(char* result[MAXDATASIZE][secondArray],char* filename, char* firstDelim, char* secondDelim){
	FILE *ptr_file;

	char buf[1000];


	char *token1;
	int i =0 ,j=0;
	ptr_file =fopen(filename,"r");
	
	if (!ptr_file)
		return 0;

	int size = 0;
	char *data[MAXDATASIZE];

	for (int i = 0; i < 1000; ++i)
	{
		if (fgets(buf,1000, ptr_file)!=NULL)
		{
			data[i]=strndup(buf,30);
			//printf("%s\n", data[i]);
			size++;
		}
	}

	//printf("%d\n", size);

	for (int i = 0; i < size; ++i)
	{
		int length = sizeof(data[i]);
		char *input;
		input = data[i];
		token1=strtok(input,firstDelim);	
		if(token1){
			result[i][j]=token1;
			//printf("%s\n", result[i][j]);
			j++;
		}	
		token1 =strtok(NULL,secondDelim);
		if (token1)
		{
			result[i][j]=token1;
			//printf("%s\n", result[i][j]);
			j--;
		}

	}
	
	fclose(ptr_file);

	return size;
}

int authUser(char* username){
	char* authentication[MAXDATASIZE][secondArray];
	int size = 0;
	int success = 0;

	size = grabFile(authentication,"Authentication.txt","\t","\n");

	for (int i = 0; i < size; ++i)
	{
		char *currentUsername = authentication[i][0];
		if (*currentUsername == *username)
		{
			success = 1;
		}
	}

	return success;		
}


int authPass(char* password){
	char* authentication[MAXDATASIZE][secondArray];
	int size = 0;
	int success = 0;

	size = grabFile(authentication,"Authentication.txt","\t","\n");

	for (int i = 0; i < size; ++i)
	{
		if (authentication[i][1] == password)
		{
			success = 1;
		}
	}

	return success;		
}

int hangman(){
	char* hangman[MAXDATASIZE][secondArray];

	int size = 0;

	size = grabFile(hangman,"hangman_text.txt",",",",");


	char* finalText = twoWords(hangman,size);	

	printf("%s\n", finalText);

	return 0;
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
