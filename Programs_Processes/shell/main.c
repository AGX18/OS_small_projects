#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 64

pid_t child_pid = -1; // Global variable to store the child process ID

void handle_SIGINT(int sig) {
    if (child_pid != -1) {
        // Terminate the child process
        kill(child_pid, SIGINT);
    }
}


int main() {
    char input[MAX_INPUT];    
    signal(SIGINT, handle_SIGINT); // Register signal handler for SIGINT

    while (1) {
        // Infinite loop to keep the program running

        printf("mysh> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break; // EOF (Ctrl+D)
        }

        input[strcspn(input, "\n")] = '\0'; // Remove trailing newline character

        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }

        printf("You typed: %s\n", input);
        int token_count = 0;
        char *tokens[MAX_TOKENS];
        char *token = strtok(input, " ");
        while (token != NULL && token_count < MAX_TOKENS - 1) {
            tokens[token_count++] = token;
            token = strtok(NULL, " \t");
        }
        tokens[token_count] = NULL; // Null-terminate the array

        if (strcmp(tokens[0], "cd") == 0) {
            // Change directory command
            if (token_count < 2) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(tokens[1]) != 0) {
                    perror("cd");
                }
            }
        } else {
            // Fork a child process to execute the command
            child_pid = fork();
            if (child_pid < 0) {
                perror("fork");
                continue;
            } else if (child_pid == 0) {
                // Child process
                execvp(tokens[0], tokens);
                perror("execvp"); // If execvp fails
                exit(EXIT_FAILURE);
            } else {
                // Parent process
                wait(NULL); // Wait for the child process to finish
                child_pid = -1; // Reset child_pid after waiting
            }
        }
    }

    return 0;
}