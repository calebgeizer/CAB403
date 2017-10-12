
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
	#define MAXDATASIZE 100
	#define authentication 2
int main() {
	FILE *ptr_file;

	char buf[1000];
	
	char *username[MAXDATASIZE][authentication];
	
	char *token1;
	int i =0 ,j=0;
	ptr_file =fopen("Authentication.txt","r");
	
	if (!ptr_file)
		return 1;

	while (fgets(buf,1000, ptr_file)!=NULL) {
		
		token1=strtok(buf,"		");		
		username[j][i]=token1;		
		printf("%s\n",username[j][i]);
		i++;	
		token1 =strtok(NULL,"		");
		username[j][i]=token1;		
		printf("%s\n",username[j][i]);
		j++;
		i--;		
		
	}
	
		
	fclose(ptr_file);
	
	
	return 0;
}
