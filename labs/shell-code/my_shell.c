#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<limits.h>
#include<signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/

int current_proc_id = -1;
int background_pid = -1;

void sigInthandler (int sig_num) {
    // signal(SIGINT, sigInthandler);
	if (current_proc_id != -1) {
		printf("\nHello\n");
		printf("%d\n", current_proc_id);
		fflush(stdout);
		kill(current_proc_id, SIGINT);
	}
	current_proc_id = -1;
    printf("\nI will run forever \n");
    fflush(stdout);
}

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}




int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int num_commands = 0;
	pid_t shell_pid = getpid();
	
	while(1) {			
		/* BEGIN: TAKING INPUT */
		int status;
		signal(SIGINT, sigInthandler);
		pid_t pid_check = waitpid(-1, &status, WNOHANG);
		printf("background ");
		printf("%d", pid_check);
		printf("\n");
		num_commands ++;
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();

		// printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them
		int numTokens = 0;
		for(i=0;tokens[i]!=NULL;i++){
			// printf("hello\n");
			// numTokens ++;
						// printf("found token %s (remove this debug output later)\n", tokens[i]);
		}
		// printf("%d",i);
		if (strcmp(tokens[0], "exit") == 0) {
			// wait(1);
			if (kill(-shell_pid, SIGTERM) == -1) {
				perror("kill");
			}
			if (background_pid != -1) {
				kill(-background_pid, SIGTERM);
			}
			// kill(-background_pid, )
			
			// while ((pid_check = waitpid(-1, &status, WNOHANG)) > 0) {
			// 	kill(pid_check, SIGTERM);
            //     printf("Terminating background process %d\n", pid_check);
            // }
            printf("Exiting shell\n");
            exit(0);
			// exit(1);
		}
		if (tokens[0] == NULL) {
			printf("No command printed. Don't fool me.\n");
			continue;
		}

		// for (int t = 0 ; t < i ; t ++) {
		// 	if (tokens[t] == NULL) break;
		// 	if (strcmp(tokens[t], "&&")) {

		// 	}
		// 	else if (strcmp(tokens[t], "&&&")) {

		// 	}
		// }

		if (tokens[0][0] == 'c' && tokens[0][1] == 'd') {
			if (tokens[1][0] == '.' && tokens[1][1] == '.') {
				char cwd[64];
				if (getcwd(cwd, sizeof(cwd)) != NULL) {
					for (int i = 63; i >= 0 ; i -- ) {
						if (cwd[i] == '/') break;
						cwd[i] = NULL;
					}
					chdir(cwd);
       				// printf("Current working dir: %s\n", cwd);
   				}

			}
			else {
				chdir(tokens[1]);
			}
			continue;
		}

		if (tokens[i-1][0] == '&') {
			// printf("hello\n");
			tokens[i-1] = NULL;
			char s[64] = "/bin/";
			strcat(s, tokens[0]);
			int rc = fork();
			if (rc == 0) {
				setpgid(0, shell_pid);

				execv(s, tokens);
				printf("This command does not exist.\n");
			}
			else {
				if (background_pid == -1) {
					background_pid = rc;
				}
				setpgid(0, background_pid);
				continue;
				// wait(1);
			}
		}

		char s[64] = "/bin/";
		strcat(s, tokens[0]);
		// printf("%s", s);
		// s += tokens[0];
		int rc = fork();
		if (rc == 0) {
			current_proc_id = getpid();
		execv(s, tokens);
		printf("This command does not exist.\n");}
		else {
			current_proc_id = rc;
			wait(1);
		}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		// if (num_commands %5 == 0) wait(1);
	}
	return 0;
}
