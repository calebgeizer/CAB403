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

void Send_Array_Data(int socket_id, char *myArray) {
    int i=0;
    char statistics;  
    /*for (i = 0; i < 2; i++) {
        printf("%c\n", myArray[i]);
        statistics = htons(myArray[i]);
        //send(socket_id, &statistics, sizeof(uint16_t), 0);
    }*/

    int length = strlen(myArray);

    send(socket_id, myArray, length, 0);
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
char* sendMessage(int argc, char *argv[],char *message){
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

    char *simpleArray = message;
    Send_Array_Data(sockfd, simpleArray); //Send Message

    // Get a response
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';
    char *response = buf;

    close(sockfd);
    return response;
}


int menu(){
    int choice;
    int x = 0;
    while(x == 0){
        printf("\n\n\n\nWelcome to the Hangman Gaming System\n\n\n\n");
        printf("\nPlease enter a selection\n<1> Play Hangman\n<2> Show Leaderboard\n<3> Quit\n\nSelection option 1-3 -> " );
        scanf("%d", &choice);
        return choice;
        if ((choice ==1)||(choice ==2)||(choice ==3))
        {
            x = 1;
        }
    }
    return choice;
}

void Hangman(char* username){
    printf("%s yes\n", username);
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

void runGame(char* name){
    printf("%s yes\n", name);

    int x = 0;
    while(x == 0){
        int choice = menu();

        if(choice ==1){
            //Play Hangman
        	Hangman(name);
        }

        else if(choice ==2){
            //Show Leaderboard
		printf("LEADERBOARD\n");
        }

        else if(choice ==3){
            //Quit
		printf("QUIT\n");
            return;
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


    printf("===========================================\n\n\nWelcome to the Online Hangman Gaming System\n\n\n===========================================");
    
    //logon
    int x = 0;
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

        char *serverResponse = sendMessage(argc,argv, passwordMessage);
        //printf("%s\n", serverResponse);

        if (serverResponse[0] == 's')
        {
            x = 1;
        }else{
            printf("\n\n\nYou entered either an incorrect username or password\n");
        }
    }

    char* name = username;
    printf("%s yes\n", name);
    runGame(name);

    return 0;
}
