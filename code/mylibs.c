#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
Split a string into substrings delimited by a given delimiter
Requires input string str, tok_list[] in which to store the substrings, max_tokens is 
the number of subtrings to store, delim is the delimeter to use when splitting the string.
The last cell in the tok_list[] array will contain NULL, maxtokens needs to be less than the 
size of the array and include the last NULL argument. The NULL substring provides compatibility 
with the exec family of functions.
*/
void str_split(char* str, char* tok_list[], int max_tokens, char* delim) {
	char* saveptr, *strptr;
	int i;

	for(i = 0, strptr = str;; i++,strptr = NULL) {
		tok_list[i] = strtok_r(strptr, delim, &saveptr);
		
		//strip off any trailing newline characters
		if (tok_list[i] != NULL) {
			
			tok_list[i][strcspn(tok_list[i], "\n")] = 0;
		}
		//str2 = &tok_list[i];
		//printf("%d %s %s\n", str2, *str2, tok_list[i]);
		//str2 = str2 + (strcspn(tok_list[i], "\n") * sizeof(char));
		//printf("%d %s %s\n", str2, *str2, tok_list[i]);

		

		if(tok_list[i] == NULL || (i >= (max_tokens-2))) 
			break;
	}

}