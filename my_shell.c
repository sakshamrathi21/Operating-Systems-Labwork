#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<limits.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
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

	while(1) {			
		/* BEGIN: TAKING INPUT */
		int status;
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
			wait(1);
			pid_t pid_curr = getpid();
			printf("Shell Code is being exited, Thank You!\n");
			kill(pid_curr);
			exit(1);
		}
		if (tokens[0] == NULL) {
			printf("No command printed. Don't fool me.\n");
			continue;
		}

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
				execv(s, tokens);
				printf("This command does not exist.\n");
			}
			else {
				
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
		execv(s, tokens);
		printf("This command does not exist.\n");}
		else {
			wait(1);
		}
       
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		if (num_commands %5 == 0) wait(1);
	}
	return 0;
}
