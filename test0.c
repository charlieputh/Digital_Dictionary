#define _CRT_SECURE_NO_WARNINGS                                          //just in case that the compiler will make errors when it comes to strcpy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Max 111111                                              	 //Max record in Dict   (rows in dict.txt / 2) 

struct dict{
	char *key;                                              	 //words  it can be used in malloc
	char *content;                                          	 //words in translation
};

/*Open the dict and read the content in it*/
int open_dict(struct dict **p, const char *dict_filename){
	FILE *pfile = fopen(dict_filename, "r");        	         //open it by READ only
	if (pfile == NULL)
		return 0;                                            	 //If open failed return 0

	*p = (struct dict *)malloc(sizeof(struct dict) * Max);   	 //Distribute The Memory
	memset(*p, 0, sizeof(struct dict)*Max);                  	 //Initial it to 0
	struct dict *pD = *p;                                            //pD point to the front address of p .It's an array

	char buf[1024] = { 0 };                                          //Cache
	size_t len = 0;                                                  //mean long unsigned int in 64bit sys
	int i = 0;                                                       //counting method
	while (!feof(pfile)){                                            //Read the file recursively 
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pfile);                          //read one line
		len = strlen(buf);                                  
		if (len > 0){
			pD[i].key = (char *)malloc(len);                 //distribute the memory according to the length
			memset(pD[i].key, 0, len);
			strcpy(pD[i].key, &buf[1]);                      //copy it into the key Notice it begins from 1  [#a]
		}

		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pfile);
		len = strlen(buf);
		if (len > 0){
			pD[i].content = (char *)malloc(len);
			memset(pD[i].content, 0, len);
			strcpy(pD[i].content, &buf[6]);
		}
		i++;
	}
	fclose(pfile);                                                     //close the file

	return i;                                                          //return the number of key
}

/*Search for the content according to the key*/
int search_dict(const struct dict *p, int size, const char *key, char *content){
	int i = 0;
	for (i = 0; i < size; i++){
		if ((p[i].key == NULL) || (p[i].content == NULL))
			continue;
		if (strncmp(p[i].key, key, strlen(key)) == 0)
		{
			strcpy(content, p[i].content);
			return 1;                                         //find the word ,return 1
		}
	}
	return 0;                                                         //if not ,return 0
}


void free_dict(struct dict *p, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)                                        //Free the memory recursively
	{
		if (p[i].key)
			free(p[i].key);
		if (p[i].content)
			free(p[i].content);
	}
	free(p);                                                          //free p
}

int main(int argc, char *args[]){
	if (argc < 2){                                     		  // if the argc is less than 2 parameter
		printf("usage : %s dict-filename\n", args[0]); 		  //test the file name
		return 0;                                      		  //parameter is not enough
	}
	long start_ms = 0;                                 		  //mark the start time
	long end_ms = 0;                                 		  //mark the end time
	struct dict * p = NULL;                            		  //Initial a pointer to the struct dict and point it to NULL 
	start_ms = clock();                                
	int size = open_dict(&p, args[1]);                 		  //open the file
	if (size == 0)
		return 0;                                      		  //failed to open,exit

	end_ms = clock();                                  
	printf("open_dict used %d ms\n", end_ms - start_ms);	          //calcalate the time used in opening the file

	char key[1024];
	char content[1024];
	while (1){                                         		  //while can make sure input constantly
		memset(key, 0, sizeof(key));
		memset(content, 0, sizeof(content));
		scanf("%s", key);                              		  //get word from the user
		if (strncmp(key, "command-exit", 12) == 0)     		  //make an exit command
			break;
		start_ms = clock();
		if (search_dict(p, size, key, content)){
			printf("%s", content);
		}
		else{
			printf("Not Found");
		}
		end_ms = clock();
		printf("search_dict used %ld ms\n", end_ms - start_ms);
	}
	start_ms = clock();
	free_dict(p, size);
	end_ms = clock();
	printf("free_dict used %ld ms\n", end_ms - start_ms);
	return 0;
}
