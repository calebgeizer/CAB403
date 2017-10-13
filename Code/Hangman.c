#include<stdio.h>
#include <stdlib.h>
#include <string.h>
	#define MAXDATASIZE 500
	#define hangman_text 2

char* twoWords(char *hangmanWords[MAXDATASIZE][hangman_text],int size){

	int i, stime;
	long ltime;

	/* get the current calendar time */
	ltime = time(NULL);
	stime = (unsigned) ltime/2;
	srand(stime);

	//select random word
	int x = rand() % size;
	int y = rand() % size;

	printf("%d %d\n", x,y);

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
/*
	while (fgets(buf,1000, ptr_file)!=NULL) {
		
		token1=strtok(buf,",");	
		hangmanWords[j][i]=token1;	
		//printf("%s\n",hangmanWords[j][i]);
		//printf("%d\n", j);
		i++;
		token1 =strtok(NULL,"\n");
		hangmanWords[j][i]=token1;		
		printf("%s\n", hangmanWords[0][0]);
		printf("%s\n",hangmanWords[j][0]);
		j++;
		i--;		
		size++;
	}
*/

	char *hangman[MAXDATASIZE];

for (int i = 0; i < 1000; ++i)
{
	if(fgets(buf,1000,ptr_file)!=NULL){
		hangman[i]=strndup(buf, 30);
		//printf("%s\n", hangman[i]);
		size++;
	}
}
	
	//printf("%d\n", size);


	for (int i = 0; i < size; ++i)
	{
		int length = sizeof(hangman[i]);
		char *input;
		input = hangman[i];
		token1=strtok(input,",");	

		if(token1){
			hangmanWords[i][j]=token1;
			//printf("%s\n", hangmanWords[i][j]);
			j++;
		}	
		token1 =strtok(NULL, ",");
		if(token1){
			hangmanWords[i][j]=token1;
			//printf("%s\n", hangmanWords[i][j]);
			j--;
		}
	}






	//printf("%d\n", size);

	char* finalText = twoWords(hangmanWords,size);
	
	printf("%s\n", finalText);
		
	fclose(ptr_file);
	
	
	return 0;
}
