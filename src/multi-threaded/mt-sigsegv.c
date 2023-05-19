#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>

#include "threading.h"

#define BUFF_LEN        (size_t)50
#define MAX_THREADS     (int)10
#define THREAD_TO_CRASH (int)5
#define CRASH_DELAY     (int)30
#define CRASH_ON_THREAD_START   1

void crasher();
void startThreads(void);

int
main( void ) {
    time_t  thisTime, tickTime;

    printf("SIGSEGV Generator...\n");

    printf("Thread to crash: %d\n", THREAD_TO_CRASH);

    startThreads();

    // Display a period every second so we know the main thread is alive.
    tickTime = thisTime =  time(0);
    for (;;) {
        thisTime = time(0);

        if (difftime( thisTime, tickTime) >= 1) {
            tickTime = thisTime;
            fprintf(stdout, ".\n");
            fflush(stdout);
        }
    }

    return 0;
}

static void *thread_run(void *arg) {
    int threadNum = *(int*)arg;

//    if (threadNum == THREAD_TO_CRASH ) {
//        fprintf( stdout, "Crashing thread %d\n", threadNum);

//        sleep(CRASH_DELAY);
//        char *nullPtr = 0;
//        *nullPtr = threadNum;
//    }

    fprintf( stdout, "Thread %d running.\n", threadNum);

    crasher(threadNum);
}

void startThreads(void) {

    unsigned char threadName[BUFF_LEN+1];
    for (int i = 0; i < MAX_THREADS; i++ ) {

        fprintf( stdout, "\nStarting thread %d\n", i);
        fflush(stdout);
        snprintf( threadName, BUFF_LEN, "Thread %d\n", i);
        sys_thread_t *thread = sys_thread(threadName, thread_run, &i);
    }

}

// Added crasher code to a function so we can experiment with data in the map file and trace.
void
crasher(int threadNum) {
    char* bogusMsg = {"The quick brown fox jumps over the lazy dog"};
    char *nullPtr = 0;
    time_t  startTime, thisTime, tickTime;

    if (threadNum == THREAD_TO_CRASH) {
        printf("Crashing thread %d in %d seconds...\n", THREAD_TO_CRASH, CRASH_DELAY);
    }

    startTime = tickTime = thisTime =  time(0);
    for (;;) {
        thisTime = time(0);

        if (difftime( thisTime, tickTime) >= 1) {
            tickTime = thisTime;
            fprintf(stdout, "<%d>", threadNum);
            fflush(stdout);
        } 
        if ((threadNum == 5) && (difftime( thisTime, startTime ) >= CRASH_DELAY)) {
            memcpy( nullPtr, &bogusMsg, strlen(bogusMsg));
        }
    }
}
