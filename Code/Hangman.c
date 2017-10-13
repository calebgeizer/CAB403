#include<stdio.h>
#include <stdlib.h>
#include <string.h>
	#define MAXDATASIZE 100
	#define hangman_text 2


char twoWords(){

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

	while (fgets(buf,1000, ptr_file)!=NULL) {
		
		token1=strtok(buf,"		");		
		hangmanWords[j][i]=token1;		
		printf("%s\n",hangmanWords[j][i]);
		i++;
	}
	
		
	fclose(ptr_file);
	
	
	return 0;
}
