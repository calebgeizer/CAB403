#include <stdio.h>
#include <ctype.h>
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

void quit() /* write error message and quit */
{
    fprintf(stderr, "memory exhausted\n");
    exit(1);
}

char* concat(char* first, char* second){
    char* concatinated;
    concatinated = malloc(strlen(first)+strlen(second)); /* make space for the new string (should check the return value ...) */
    strcpy(concatinated, first); /* copy name into the new var */
    strcat(concatinated, second); /* add the extension */
    return concatinated;
}

char* Send_Data(int socket_id, char *message) {
    int i=0;
    char statistics;  
    /*for (i = 0; i < 2; i++) {
        printf("%c\n", message[i]);
        statistics = htons(message[i]);
        //send(socket_id, &statistics, sizeof(uint16_t), 0);
    }*/

    int length = strlen(message);

    send(socket_id, message, length, 0);
}
char* Send_Menu(int socket_id, char *message) {
    int i=0;
    char statistics;  
   

    int length = strlen(message);

    send(socket_id, message, length, 0);
}

// asks the user to respond
char* response(){
    int max = 20;
    char* name = (char*) malloc(max); /* allocate buffer */
    char* response;
    if (name == 0) quit();

    while (1) { /* skip leading whitespace */
        int c = getchar();
        if (c == EOF) break; /* end of file */
        if (!isspace(c)) {
             ungetc(c, stdin);
             break;
        }
    }

    int i = 0;
    while (1) {
        int c = getchar();
        if (isspace(c) || c == EOF) { /* at end, add terminating zero */
            name[i] = 0;
            break;
        }
        name[i] = c;
        if (i == max - 1) { /* buffer full */
            max += max;
            name = (char*) realloc(name, max); /* get a new and larger buffer */
            if (name == 0) quit();
        }
        i++;
    }
    response = name;
    free(name); /* release memory */
    return response;
}

// Sends the server a message and gets a response




void Hangman(char* username){
    
    int x = 0;
    char *choice = "0";
    while(x == 0){
        printf("\n\nGuessed letters:\n\nNumber of gusses left:");
        //get the number of guesses
        printf("\n\nWord:\n");
        //get the word
        printf("\n\nEnter your guess - ");
        choice = response();
	
        printf("\n\n-------------------------------------------\n");

        //TEMP QUIT
        if((strcmp(choice,"Q") == 0)){
            x = 1;
        }
    }

    printf("\n\nGame Over\n");
    printf("\n\n\nWell done %s! You won this round of Hangman!\n",username);
}


char* runGame(int socket_id,char* name){
	char choice[5];
	
	int x = 0;
   	while(x == 0){

		printf("\n\n\n\nWelcome to the Hangman Gaming System\n\n\n\n");
		printf("\nPlease enter a selection\n<1> Play Hangman\n<2> Show Leaderboard\n<3> Quit\n\nSelection option 1-3 -> " );
		fgets(choice, 10, stdin);
		choice[strlen(choice) - 1] = '\0';
		printf("\n%c\n",choice[0]);
		if(choice[0] == '1'){
		    //Play Hangman
			printf("\n%c\n",choice[0]);
			Send_Data(socket_id, choice);
			Hangman(name);
			x =1;
		}
		else if(choice[0] == '2'){
		    //Show Leaderboard
			printf("LEADERBOARD\n");
			Send_Data(socket_id, choice);
			x =1;
		}
		else if(choice[0] == '3'){
		    //Quit
			printf("QUIT\n");
			Send_Data(socket_id, choice);
			x =1;
		    
		}
		else {
			printf("Please enter a valid number 1 -3\n");	
		}
    }
    

}



int main(int argc, char *argv[])
{
	char text2[100];
	char text[100];
	char* username = "none";
	int sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information*/
	int x=0;


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


	printf("===========================================\n\n\nWelcome to the Online Hangman Gaming System\n\n\n===========================================");
	//logon
	
	
	while(x == 0){

		printf("\n\n\nYou are required to logon with your registered Username and Password\n\n");
		printf("Please enter your username -->");
		fgets(text2, 100, stdin);
		text2[strlen(text2) - 1] = '\0';
		username = text2;

		printf("Please enter your password -->");
		fgets(text, 100, stdin);
		text[strlen(text) - 1] = '\0';
		char *passwordMessage = concat("b",text);

		passwordMessage = concat(passwordMessage,username);
		Send_Data(sockfd, passwordMessage);
		
		if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        	perror("recv");
        	exit(1);
    	}
		
		char *serverResponse = buf;
		printf("%s\n", serverResponse);
		if (serverResponse[0] == 's')
		{
		    x = 1;
		}else{
		    printf("\n\n\nYou entered either an incorrect username or password\n");
		}
	}


	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("socket");
	exit(1);
	}

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
	perror("connect");
	exit(1);
	}

	char* name = username;
	runGame(sockfd,name);

	return 0;
}
