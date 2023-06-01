#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile int count = 0;

void sigusr1_handler(int signum) {
    count++;
}

void sigusr2_handler(int signum) {
    printf("Count: %d\n", count);
}

int main() {
    struct sigaction sa1, sa2;
    sa1.sa_handler = sigusr1_handler;
    sa1.sa_flags = 0;
    sigemptyset(&sa1.sa_mask);

    sa2.sa_handler = sigusr2_handler;
    sa2.sa_flags = 0;
    sigemptyset(&sa2.sa_mask);
    
    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("Error registering SIGUSR1 handler");
        return 1;
    }

    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("Error registering SIGUSR2 handler");
        return 1;
    }

    printf("Waiting for signals...\n");

    while (1) {
	    char input;
	    printf("enter 1 for add count, 2 for print, q for quit:");
	    scanf(" %c", &input);

	    if (input == '1') {
		    kill(getpid(), SIGUSR1);
	    } else if (input == '2') {
		    kill(getpid(), SIGUSR2);
	    } else if (input == 'q') {
		    break;
	    }    
    }

    return 0;
}

