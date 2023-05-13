#include <stdio.h>
#include <memory.h>
#include <time.h>

void crasher();

int
main( void ) {
    printf("SIGSEGV Generator.\n");

    crasher();

    return 0;
}

// Added crasher code to a function so we can experiment with data in the map file and trace.
void
crasher() {
    char* bogusMsg = {"The quick brown fox jumps over the lazy dog"};
    char *nullPtr = 0;
    int crashDelay = 30;
    time_t  startTime, thisTime, tickTime;

    printf("Crashing in %d seconds...\n", crashDelay);

    startTime = tickTime = thisTime =  time(0);
    for (;;) {
        thisTime = time(0);

        if (difftime( thisTime, tickTime) >= 1) {
            tickTime = thisTime;
            printf(".\n");
        }
        else if (difftime( thisTime, startTime ) >= crashDelay) {
            memcpy( nullPtr, &bogusMsg, strlen(bogusMsg));
        }
    }
}
