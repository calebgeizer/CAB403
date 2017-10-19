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

int checker(char first[], char second[]){
	int result;
	for(int i=0; first[i]!='\0' && second[i]!='\0'; i++) {
		if(first[i] !=second[i]) {
			result =0;
			break;
		}
		else{
			result =1;
		}
	}
	return result;
}

void Hangman(char* username,char* word){
    
	int x = 0;
	int count = 0;
	int chk;
    //get the number of guesses
	int numGuessLeft = strlen(word)-1 + 10;

	if(numGuessLeft > 26){
	numGuessLeft = 26;
	}
	
	char correct[strlen(word)];
	
	char shownWord[strlen(correct)];
	char guessedLetters[numGuessLeft+1];
	
	for (int i = 0; i < strlen(word); ++i) {
		if(isalpha(word[i])) {
			correct[i] = word[i];
			
		}
		else {
			correct[i]=' ';
		}			
	
	}
	for (int i = 0; i < strlen(correct); ++i) {
		if(isalpha(correct[i])) {
			shownWord[i]='_';
			
		}
		else {
			shownWord[i]=' ';
		}	
	}
	while(x == 0){
		
		printf("\n\nGuessed letters:%s",guessedLetters);
		printf("\n\nNumber of gusses left: %d", numGuessLeft);
		printf("\n\nWord: %s ",shownWord);
		printf("\n\nWord: %s ",correct);
		char answer[10];
		printf("\n\nEnter your guess - ");
		fgets(answer, 10, stdin);
		answer[strlen(answer) - 1] = '\0';
		for (int i =0; i < strlen(correct); ++i) {
			if(answer[0] == correct[i]) {
				shownWord[i] = answer[0];
			}
		}
		guessedLetters[count] = answer[0];
		numGuessLeft--;
		count++;
		guessedLetters[count] = '\0';
		printf("\n\n-------------------------------------------\n");
		chk=checker(shownWord,correct);
		printf("\nchecking %d", chk);
		if(chk==1) {
			printf("\nGame Over");
			printf("\n\n\nWell done %s! You won this round of Hangman!\n",username);
				x=1;
		}
		if(numGuessLeft == 0){
			if(chk==1) {
				printf("\nGame Over");
				printf("\n\n\nWell done %s! You won this round of Hangman!\n",username);
				x=1;
			}
			else {
				printf("\nGame Over");			
				printf("\n\n\nBad luck %s! You have run out of guesses. The Hangman got you!\n",username);	
				x=1;	
			}
		}
		

		
	}
			

	  


}


char* runGame(int socket_id,char* name){
	char choice[5];
	char buf[MAXDATASIZE];
	int numbytes;
	
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

			if ((numbytes=recv(socket_id, buf, MAXDATASIZE, 0)) == -1) {
	        	perror("recv");
	        	exit(1);
	    	}
			buf[strlen(buf)] = '\0';
			

			
			char* word = buf;

			word[strlen(word)] = '\0';

			
			Hangman(name, word);
			x = 1;
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

int newRequest(int sockfd, struct hostent *he, struct sockaddr_in their_addr, int port){

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

	return sockfd;

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



	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(port);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */


	sockfd = newRequest(sockfd,he,their_addr, port);


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
		printf("%s\n", passwordMessage);
		Send_Data(sockfd, passwordMessage);
		
		if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        	perror("recv");
        	
    		}
		
		char *serverResponse = buf;
		printf("%s\n", serverResponse);
		if (serverResponse[0] == 's')
		{
		    x = 1;
		}else{
			printf("\n\n\nYou entered either an incorrect username or password\n");
			sockfd = newRequest(sockfd,he,their_addr, port);		
		}
	}

	sockfd = newRequest(sockfd,he,their_addr, port);

	char* name = username;
	runGame(sockfd,name);

	return 0;
}
