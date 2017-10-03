/*
pman.c

CSC360
Assignment 1
Mark Worrall
Oct. 2/17 

* Program needs to display prompt and wait for user input
* -Implemented
* -Need to parse the input for the path and the command name (if included) and send an error if the 
*  input is invalid.
* -Save input string and the parse it for commands and paths
* 
* If user inputs a program path, that program executes. Can include the "bg" tag which will 
*    open the program in the background and display prompt.
* -need to fork the PMan program and then run execvp() with the input path from the child process
*  to start the requested process.
* 
* The user can also input a number of commands:
*   bgkill [pid] - terminate the process with the given pid
*   bgstop [pid] - stops the given process
*   bgstart [pid] - starts the given process
*   pstat [pid] - displays a number of stats for the given process.
*   bglist - lists all of the processes currently running.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "mylibs.h"
#include "linked_list.h"
#include <signal.h>


int num_progs = 0;
int list_len = 10;


/*
*Executes a program using the execvp function.
*Requires an array of args to pass to the execvp function. This first part of the array
*should contain the program path, and the subsequent cells should contain the args for
*that programn. The final cell of the array needs to be NULL. 
*run_code is 0 for normal, and 1 for background execution
* pids are stored in linked list L.
*/
int run_program (char* args[], LL* L , int run_code) {
	//int status;
	int file = open(args[0], O_RDONLY );
	if (file==-1) {
		printf("Could not open file: %s\n", args[0]);
		return 0;
	} else {
		close(file);
	}

	
	if(args[0]==NULL)
		return 0;

	pid_t pid = fork();
	
	//Parent process         
	

	//Child process
	if (pid == 0) {	 		
		execvp(args[0], args);
		printf("%s\n", "\nexecvp failed: exiting\n");
		exit(0);

	} else if(pid > 0){
		addNode(L, pid);

	}
	return 0; //(status);
}


//read the list of pids that have been added to the list and
	//list them.
void bglist(LL* L) {
	size_t path_size = 200;
	char pid[10];
	char path_name[200];
	char path_buff[200];
	char* str1 = "/proc/";
	char* str2 = "/exe";
	int pidc = getSize(L);

	strcpy(path_name, str1);
	printf("\n");

	for(int i = 0;i < pidc;i++) {
		sprintf(pid, "%d", getIndex(L, i)->element);		
		strcpy(path_name, str1);
		strcat(path_name, pid);
		strcat(path_name, str2);

		readlink(path_name, path_buff, path_size);
		strcat(path_buff, "\0");

		printf("%s: %s\n", pid, path_buff);
	}
	printf("Total background jobs: %d\n\n", pidc);
}


int bgkill(LL* L, int pid) {
	int killret = kill(pid, SIGTERM);

	if(killret == -1) {
		printf("Error: could not kill process %d\n", pid );
		return -1;
	}	

	removeElement(L, pid);
	if(killret == 0) printf("Process ID %d has terminated successfully.\n", pid);
	return 0;
}

int bgstop(int pid) {
	int killret = kill(pid, SIGSTOP);
	if(killret != 0) {
		printf("Error: Could not stop process.\n");
		return 0;
	}
	return 1;

}

int bgstart(int pid) {
	int status;
	waitpid(pid, &status, WNOHANG);
	int killret = kill(pid, SIGCONT);
	if(killret != 0) {
		printf("Error: Could not stop process.\n");
		return 0;
	}
	return 1;
	
}


/*
Lists information for the given pid
(2) Name of program 
(3) Current state
(14) utime (user time)
(15) stime (kernel mode)
(24) RSS
voluntary_ctxt_switches
nonvoluntary_ctxt_switches
*/
int pstat(char* pid) {
	char path[50];
	char path2[50];
	char* str = "/proc/";
	char* str2 = "/stat";
	char* str3 = "/status";
	char stat_buff[1000];
	char status_buffer[200];
	int max_size = 52;
	char* stat_data[max_size+1];
	int counter = 51;

	strcpy(path, str);
	strcat(path, pid);
	strcpy(path2, path);
	strcat(path2, str3);
	strcat(path, str2);
	FILE* stat = fopen(path, "r");
	FILE* status = fopen(path2, "r");

	fgets(stat_buff, sizeof(stat_buff), stat);

	str_split(stat_buff, stat_data, max_size, " ");

	printf("\nPstat\n==============\n");
	printf("program name: %s\ncurrent state: %s\n", stat_data[1], stat_data[2]);
	printf("utime: %.4f s\nstime: %.4f s\n", ((float)atoi(stat_data[13])/sysconf(_SC_CLK_TCK)), ((float)atoi(stat_data[14])/sysconf(_SC_CLK_TCK)));
	printf("resident set size (RSS): %s\n", stat_data[23]);

	while(counter >= 0) {
		fgets(status_buffer, sizeof(status_buffer), status);
		if(strncmp(status_buffer, "voluntary_ctxt_switches", 10)==0) {
			printf("%s", status_buffer);
		}
		if(strncmp(status_buffer, "nonvoluntary_ctxt_switches", 10)==0) {
			printf("%s", status_buffer);
			break;
		}
		counter--;
	}
	printf("\n" );
	fclose(stat);
	fclose(status);

	return 1;
}

/*
*************Main Starts Here*************
==========================================
*/
int main(int argc, char* argv[]) {
	


	if(DEBUG==1) printf("\n%s \n\n", "Running in Debug mode.");
		
 
	int max_args = 10;	
	char* myargs[max_args+1];
	char* delim = " ";
	size_t str_len = 0;
	int status;
	
	//Create and initialize a linked list to hold pids
	LL PIDs;	
	initList(&PIDs);

	char prompt_st[10] = "PMan: >";

	while(1) {
		
		//initialize input buffer, command buffer and input_ptr
		char input[100] = "";
		char command[15] = "";
		char *input_ptr;
		
		waitpid(-1,&status, WNOHANG);/*Clean up any zombie process still around*/

		//Loop for checking the contents of pid_list
		if(DEBUG==1) {
			for(int i = 0; i < list_len; i++) {
				printf("%d \n", getIndex(&PIDs, i)->element);
			}
		}

		//Initialize arg list to NULL
		for (int i = 0; i<max_args; i++) {
			myargs[i] = NULL;
		}

		//read user input and store first space delimited string as command
		printf("%s ",prompt_st);
		fgets(input, sizeof(input), stdin);
		sscanf(input, "%s", command);

		//if there are strings after the command, move the str pointer to the next arg after the 
		//command string.
		str_len = strlen(input);
		if (str_len>0) {
			input_ptr = &input[strcspn(input, " ")];
		}

		/*
		//put first argument in a separate variable, remove from the rest of the args,
			//use sscanf to get the first string
			//move the string pointer to the point after the first arg
				//use strcspn(input, " ") to get the location of the first space
		//then parse the list into tokens and put them in the arg array.
		*/

		//Split the strings based on the delimiter (space in this case), and place
		//each in a cell in an array. 
		str_split(input_ptr, myargs, max_args, delim);

		//list args for  debugging
		if(DEBUG==1) {
			for (int i = 0; i<max_args; i++) {
				printf("Arg %d: %s\n", i, myargs[i]);
			}
		}
	//**********start of command recognizer**********
		
		if(strcmp(command, "bg") == 0) {				//BG command
			if(DEBUG==1) printf("%s\n", "This is the BG command.");
			char **ptr = &myargs[0];
			run_program(ptr, &PIDs, 1);

		} else if(strcmp(command, "bgkill") == 0) { 	//BGKILL command
			if(DEBUG==1) printf("%s\n", "This is the bgkill function");
			if(myargs[0]==NULL) {
				printf("%s\n", "Usage: bgkill [pid].");
			} else if(inList(&PIDs, atoi(myargs[0])) == NULL) {
				printf("Error: process %s does not exist\n", myargs[0]);
			} else {
				bgkill(&PIDs,atoi(myargs[0]));
			}	

		} else if(strcmp(command, "bgstop") == 0) {	//BGSTOP command
			if(DEBUG==1) printf("%s\n", "This is the bgstop function.");
			if(myargs[0]==NULL) {
				printf("%s\n", "Usage: bgstop [pid].");
			} else if(inList(&PIDs, atoi(myargs[0])) == NULL) {
				printf("Error: process %s does not exist\n", myargs[0]);
			} else {
				bgstop(atoi(myargs[0]));
			}	

		} else if(strcmp(command, "bgstart") == 0) {	//BGSTART command
			if(DEBUG==1) printf("%s\n", "This is the bgstart command.");
			if(myargs[0]==NULL) {
				printf("%s\n", "Usage: bgstart [pid].");
			} else if(inList(&PIDs, atoi(myargs[0])) == NULL) {
				printf("Error: process %s does not exist\n", myargs[0]);
			} else {
				bgstart(atoi(myargs[0]));
			}	
		
		} else if(strcmp(command, "bglist") == 0) {	//BGLIST command
			if(DEBUG==1) printf("%s\n", "This is the bglist command.");
			bglist(&PIDs);
			

		} else if(strcmp(command, "pstat") == 0) {		//PSTAT command
			if(DEBUG==1) printf("%s\n", "This is the pstat command.");
			if(myargs[0]==NULL) {
				printf("%s\n", "Usage: pstat [pid].");
			} else if(inList(&PIDs, atoi(myargs[0])) == NULL) {
				printf("Error: process %s does not exist\n", myargs[0]);
			} else {
				pstat(myargs[0]);
			}
			
		
		} else { 										//ERROR state, cannot run input
			printf("%s: command not found.\n", command);
		
		}
	//**********end of command recognizer**********
	}

}