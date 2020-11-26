#include <time.h>
#include "defs.h"

struct timespec ts_start;
struct timespec ts_end;

double totalTime(struct timespec start, struct timespec end) {
    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    return time_taken;
}

void timerStart() { clock_gettime(CLOCK_MONOTONIC, &ts_start); }
void timerEnd() { clock_gettime(CLOCK_MONOTONIC, &ts_end); }
// void timerPrint(char *argv) { DEBUG_PRINT(("Time to run %s: [%lf]s\n", argv, totalTime(ts_start, ts_end))); }
void timerPrint(char argv[]) { printf("Time to run %s: [%lf]s\n", argv, totalTime(ts_start, ts_end)); }

void timerStartCustom(struct timespec &ts_start) { clock_gettime(CLOCK_MONOTONIC, &ts_start); }
void timerEndCustom(struct timespec &ts_end) { clock_gettime(CLOCK_MONOTONIC, &ts_end); }
void timerPrintCustom(struct timespec start, struct timespec end, char argv[]) {
    printf("Time to run %s: [%lf]s\n", argv, totalTime(start, end));
}
