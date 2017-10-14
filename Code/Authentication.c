#include<stdio.h>
#include <stdlib.h>
#include <string.h>

	#define MAXDATASIZE 500
	#define secondArray 2

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

void authentication(char* username, char* password){
	char* authentication[MAXDATASIZE][secondArray];
	int size = 0;

	size = grabFile(authentication,"Authentication.txt","\t","\n");

		
}

void hangman(){
	char* hangman[MAXDATASIZE][secondArray];

	int size = 0;

	size = grabFile(hangman,"hangman_text.txt",",",",");


	char* finalText = twoWords(hangman,size);	

	printf("%s\n", finalText);
}

int main() {

	
	return 0;
}
