#include<stdio.h>
#include <stdlib.h>
#include <string.h>
	#define MAXDATASIZE 500
	#define hangman_text 10

char twoWords(char *hangmanWords[MAXDATASIZE][hangman_text],int size){

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
	strcat(finalText, word2); /* add the extension */

	//printf("%s\n", finalText);
	return *finalText;
}

int main() {
	FILE *ptr_file;

	char buf[1000];
	
	char *hangmanWords[MAXDATASIZE][hangman_text];
	
	char *token1;
	int i =0 ,j=0;
	ptr_file =fopen("hangman_text.txt","r");
	
	if (!ptr_file)
		return 1;

	int size = 0;

	while (fgets(buf,1000, ptr_file)!=NULL) {
		
		token1=strtok(buf,",");		
		hangmanWords[j][i]=token1;		
		printf("%s\n",hangmanWords[j][i]);
		i++;
		token1 =strtok(NULL,"		");
		hangmanWords[j][i]=token1;		
		printf("%s\n",hangmanWords[j][i]);
		j++;
		i--;		
		size++;
	}


	
	for (int i = 0; i < size; ++i)
	{
		printf("%s\n", hangmanWords[0][i]);
	}

	

	//printf("%d\n", size);

	char finalText = twoWords(hangmanWords,size);
	
		
	fclose(ptr_file);
	
	
	return 0;
}
