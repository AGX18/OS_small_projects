#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

void handle_winch(int sig) {
    printf("\033[H\033[J");
}


void draw_unicode_box(int width, int height) {
    // Unicode box-drawing characters
    const char* horizontal = "─";    // Horizontal line
    const char* vertical = "│";      // Vertical line
    const char* top_left = "┌";      // Top-left corner
    const char* top_right = "┐";     // Top-right corner
    const char* bottom_left = "└";   // Bottom-left corner
    const char* bottom_right = "┘";  // Bottom-right corner
    

    for (int j = 0; j < height / 2; j++) {
            printf("\n");
    }

    for (int j = 0; j < width / 2; j++) {
            printf(" ");
    }
    // Draw the top border
    printf("%s", top_left);
    for (int i = 0; i < width - 2; i++) {
        printf("%s", horizontal);
    }
    printf("%s\n", top_right);
    
    // Draw the middle rows
    for (int i = 0; i < height - 2; i++) {
        for (int j = 0; j < width / 2; j++) {
                printf(" ");
        }
        printf("%s", vertical); // Left border
        
        // Fill the middle with spaces
        for (int j = 0; j < width - 2; j++) {
            printf(" ");
        }
        
        printf("%s\n", vertical); // Right border
    }
    
    for (int j = 0; j < width / 2; j++) {
            printf(" ");
    }
    // Draw the bottom border
    printf("%s", bottom_left);
    for (int i = 0; i < width - 2; i++) {
        printf("%s", horizontal);
    }
    printf("%s\n", bottom_right);
}

int main() {

    signal(SIGWINCH, handle_winch);

    struct winsize w;

    
    do {
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            perror("ioctl");
            return 1;
        }
        draw_unicode_box(w.ws_col / 2, w.ws_row / 2);
        pause();  // Wait for signals
    } while (1);

    return 0;
}


