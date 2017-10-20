/*
*  Materials downloaded from the web. See relevant web sites listed on OLT
*  Collected and modified for teaching purpose only by Jinglan Zhang, Aug. 2006
*/
#include <time.h>    
#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stddef.h> // for NULL



	#define NAME_LENGTH 256
	#define PEOPLE 10

	#define MAXDATASIZE 500
	#define secondArray 2

	#define MYPORT 12345    /* the port users will be connecting to */
	#define ARRAY_SIZE 50 

	#define BACKLOG 10     /* how many pending connections queue will hold */

void sighandler(int);

struct person 
{
    char *name;
    int won ;
    int played;
};

char *Receive_Data(int socket_identifier, int size) {
	int number_of_bytes, i=0;
	uint16_t statistics;


	int sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information*/

	if ((numbytes=recv(socket_identifier, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	char *results = buf;

	return results;
}
char *Receive_Menu(int socket_identifier, int size) {
	int number_of_bytes, i=0;

	int sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information*/
	if ((numbytes=recv(socket_identifier, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	buf[numbytes] = '\0';
	char *results = buf;
	return results;
}


struct leaderboard
{
	char* clientName;
	int gamesPlayed;
	int gamesWon;
};

void leaderboards(){
     int i;
     struct leaderboard record[2];

     // 1st student's record
     record[0].gamesPlayed=1;
     strcpy(record[0].clientName, "Raju");

     // 2nd student's record         
     record[1].gamesPlayed=2;
     strcpy(record[1].clientName, "Surendren");

     // 3rd student's record
     record[2].gamesPlayed=3;
     strcpy(record[2].clientName, "Thiyagu");

     for(i=0; i<3; i++)
     {
         printf(" gamesPlayed: %d \n", record[i].gamesPlayed);
         printf(" Name is: %s \n", record[i].clientName);
     }

}

char* concat(char* first, char* second){
	char* concatinated;
	concatinated = malloc(strlen(first)+strlen(second)); /* make space for the new string (should check the return value ...) */
	strcpy(concatinated, first); /* copy name into the new var */
	strcat(concatinated, second); /* add the extension */
	return concatinated;
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


	char *word1 = hangmanWords[x][1];
	char *word2 = hangmanWords[x][0];

	
	char*finalText = concat(word1, "\t");
	finalText = concat(finalText, word2);

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
			data[i]=strndup(buf,50);
			size++;
		}
	}

	for (int i = 1; i < size; ++i)
	{
		int length = sizeof(data[i]);
		char *input;
		input = data[i];
		token1=strtok(input,firstDelim);
	
		if(token1){
			result[i][j]=token1;
			j++;
		}	
		token1 =strtok(NULL,secondDelim);

		if (token1)
		{
			result[i][j]=token1;
			j--;
		}

	}
	
	fclose(ptr_file);

	return size;
}

int count(char* text){
	int size = 0;
		
	for (int i = 0; i < sizeof(text); ++i)
	{

		if (isalpha(text[i]))
		{
			size++;
		}
	}

	return size;
}

int authUser(char* username){
	char* authentication[MAXDATASIZE][secondArray];
	int size = 0;
	int pos = 0;
	char* success = NULL;

	size = grabFile(authentication,"Authentication.txt","\t","\n");

	for (int i = 1; i < size; ++i)
	{
		char *currentUsername = authentication[i][0];
		
		//get length of compared words
		int userCount = count(username);
		int currentCount = count(currentUsername);

		int count = 0;
		
		//check character similarity
		for (int j = 0; j < userCount; ++j)
		{
			if(currentUsername[j] == username[j]){
				count++;
			}
		}


		if (count == userCount && userCount == currentCount)
		{
			success = username;
			pos = i;
		}
	}

	return pos;		
}

int authPass(char* password, int pos){
	char* authentication[MAXDATASIZE][secondArray];
	int size = 0;
	int success = 0;
	char finalPass[6];
	size = grabFile(authentication,"Authentication.txt","\t","\n");
	char *currentPassword = authentication[pos][1];
	while (isspace(*currentPassword))
		++currentPassword;
	int count = 0;

	//check character similarity
	for (int j = 0; j < 6; ++j)
	{

		if(currentPassword[j] == password[j]){
			count++;
		}
	}

	if (count == 6)
	{
		success = 1;
	}

	return success;			
}

char* hangman(){
	char* hangman[MAXDATASIZE][secondArray];

	int size = 0;

	size = grabFile(hangman,"hangman_text.txt",",","\n");


	char* finalText = twoWords(hangman,size);	
	finalText[strlen(finalText)] = '\0';
	printf("%s\n", finalText);

	return finalText;
}
char* checkMenu(char* menu) {
	char *result;

	if(menu[0] == '1'){
		printf("Play Hangman\n");
		result = hangman();
	}

	if(menu[0] == '2'){
		printf("Show Leaderboard\n");
		leaderboards();
	}

	if(menu[0] == '3'){
		printf("Quit\n");
	}

	return result;
}

char* checkMessage(char* message){
	//checks what the message is to give the client a response


	char label[3];
	char pass[6];
	char username[8];
	char *user;
	label[0] = message[0];
	

	//if authentication
	if (label[0] == 'b')
	{
		char *text;
		text = message + 1;
		user = message + 7;

		for (int i = 0; i < 8; ++i)
		{
			username[i] = user[i];
		}

		for (int i = 0; i < 7; ++i)
		{

			if (i < 6)
			{
				pass[i] = text[i];
			}

			if (i == 6)
			{
				pass[i] = '\0';
			}

		}

		int userPos = authUser(username);
		int passSuc = authPass(pass, userPos);

		if (passSuc == 1)
		{
			return "success";
			
			
		}

		return "fail";
	}
	

	return message;
}

char* Message(char* message){
	//checks what the message is to give the client a response


	char label[3];
	char pass[6];
	char username[8];
	char *user;
	label[0] = message[0];
	

	//if authentication
	if (label[0] == 'b')
	{
		char *text;
		text = message + 1;
		user = message + 7;

		for (int i = 0; i < 8; ++i)
		{
			username[i] = user[i];
		}

		for (int i = 0; i < 7; ++i)
		{

			if (i < 6)
			{
				pass[i] = text[i];
			}

			if (i == 6)
			{
				pass[i] = '\0';
			}
		}
	}

	return user;
}
int main(int argc, char *argv[])
{
	int sockfd, new_fd,port;  /* listen on sock_fd, new connection on new_fd */
	struct sockaddr_in my_addr;    /* my address information */
	struct sockaddr_in their_addr; /* connector's address information */
	socklen_t sin_size;
	char *results;
	char *menu;
	int id = 0;

	signal(SIGINT, sighandler);

	if (argc <= 1)
	{
		port = MYPORT;
	}

	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
	else port = MYPORT;

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
	struct person per[PEOPLE];
	
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

			results = Receive_Data(new_fd,  ARRAY_SIZE);
			printf("%s -Client sent\n", results);
			
			char* answer;
			char* menu_answer;
			char* name;
			
			if (results[0] == 'b')
			{
				answer = checkMessage(results);
				name = Message(results);
				

				if(answer[0] == 's') {
					printf("%s accessing\n", name);
					per[id].name = name;
					//printf("%s person\n", per[id].name);
				}

				if (send(new_fd, answer, sizeof(answer), 0) == -1)
					perror("send");

				

			}else{

				menu_answer = checkMenu(results);
				if (strcmp(menu_answer,"1") == 0) {
					per[id].played++;
				}
				int len = strlen(menu_answer) +1;
				//menu_answer =malloc(len);
				printf("%lu", strlen(menu_answer));

				if (strcmp(results,"won") == 0) {
					per[id].won++;
				}

				if (send(new_fd, menu_answer, len, 0) == -1)
					perror("send");

				free(menu_answer);

			}

			
			close(new_fd);
			exit(0);
		}
		
		close(new_fd);  /* parent doesn't need this */
		
		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
		
	}
	id++;	
}

void sighandler(int signum) {
   printf("\n\n closing server...\n\n");
   exit(1);
}
