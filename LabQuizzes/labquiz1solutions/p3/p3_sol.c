// THIS CODE CONTAINS SOLUTIONS TO ALL THE SUB-PARTS, COMMENTS HAVE BEEN ADDED TO ISOLATE THE CODES OF EACH OF THE SUB-PARTS

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_NUM_STRINGS 64

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;
    for (i = 0; i < strlen(line); i++)
    {

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;
    // printf("line: %s\n",line);
    // for(int i=0;i<tokenNo;i++){
    //     printf("token: %s\n",tokens[i]);
    // }
    return tokens;
}

void reap_children(int signal_num)
{
    int x;
    while((x=waitpid(-1,NULL,WNOHANG))>0){}
}


/* THE MAIN FUNCTION */
int main(int argc, char *argv[])
{
    char line[MAX_INPUT_SIZE];
    char **tokens;
    int i;
    signal(SIGCHLD, reap_children);

    while (1)
    {
        /* BEGIN: TAKING INPUT */
        bzero(line, sizeof(line));
        printf("$ ");
        scanf("%[^\n]", line);
        getchar();
        /* END: TAKING INPUT */

        /*Creating an array of strings from the input line
        Eg,
        For the line "sleep 10 &&& pwd"
        strings[0] is "sleep 10 "
        and strings[1] is " pwd"*/
        char *strings[MAX_NUM_STRINGS];
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, "&&&");
        int numStrings = 0;

        while (token != NULL && numStrings < MAX_NUM_STRINGS)
        {
            strings[numStrings] = (char *)malloc(strlen(token) + 1);
            strcpy(strings[numStrings], token);
            strings[numStrings][strlen(strings[numStrings])] = '\n'; // terminate with new line
            numStrings++;
            token = strtok(NULL, "&&&");
        }
        /*Uncomment the below snippit to see what is stored in strings*/
        // for (int i = 0; i < numStrings; i++) {
        //     printf("String %d: %s\n", i + 1, strings[i]);
        // }

        if(numStrings==1){
            tokens = tokenize(strings[0]);
            if (tokens[0] == NULL) // if empty command, ask for a command again in new line
                continue;
            int f = fork();
            if (f == 0) // Child process will execute this part
                execvp(tokens[0], tokens);
            else   
                waitpid(f,NULL,NULL);
        }else{
            for (int i = 0; i < numStrings; i++)
            {
                tokens = tokenize(strings[i]);
                if (tokens[0] == NULL) // if empty command, ask for a command again in new line
                    continue;
                int f = fork();
                if (f == 0) // Child process will execute this part
                    execvp(tokens[0], tokens);
            }
        }
        // Freeing the allocated memory to the strings
        for (int i = 0; i < numStrings; i++)
        {
            free(strings[i]);
        }
        // Freeing the allocated memory to the tokens
        for (int i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }
    return 0;
}