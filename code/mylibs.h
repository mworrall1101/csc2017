/*

Declarations for functions that are part of my personal c-library.

Mark Worrall - 09/27/17

*/


/*
Preconditions: Any empty array cells are assumed to contain NULL.
Postconditions: An array that contains a number of delimeted substrings from the input string
	up to the number specified by myargsc.

Takes in a string ponted to by str, and separates it into a list of tokens split based on the 
contents of delim. This has only been tested by single character delimiters. The tokens are 
saved in the list pointed to by tok_list. The myargsc int prevents reading past the end of the 
tok_list array and should be equal to the length of the (tok_list array-1), leaving the last
element of the array NULL.

*/
void str_split(char* str, char* tok_list[], int myargsc, char* delim);