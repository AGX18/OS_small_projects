#define _GNU_SOURCE
#include <sched.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>

#define SLEEP_SEC 3
#define NUM_MULS 100000000
#define NUM_MALLOCS 100000
#define MALLOC_SIZE 1000

// TODO define this struct
struct profile_times {
    struct timespec start_time;  
    struct rusage usage;
};

// TODO populate the given struct with starting information
void profile_start(struct profile_times *t) {
    clock_gettime(CLOCK_MONOTONIC, &t->start_time);
    getrusage(RUSAGE_SELF, &t->usage);  // Get resource usage information
}

void print_pid_cpu() {
    pid_t pid = getpid();
    printf("[pid %d, cpu %d] ", pid, sched_getcpu());
}

// TODO given starting information, compute and log differences to now
void profile_log(struct profile_times *t) {
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);  // Get current time
    struct rusage end_usage;
    getrusage(RUSAGE_SELF, &end_usage);  // Get resource usage at the end
    
    double elapsed = (end_time.tv_sec - t->start_time.tv_sec) + (end_time.tv_nsec - t->start_time.tv_nsec) / 1e9;  // Calculate elapsed time in seconds
    
    double user_time = (end_usage.ru_utime.tv_sec - t->usage.ru_utime.tv_sec) 
        + (end_usage.ru_utime.tv_usec - t->usage.ru_utime.tv_usec) / 1e6;  // Calculate user CPU time
    
        double sys_time = (end_usage.ru_stime.tv_sec - t->usage.ru_stime.tv_sec) 
        + (end_usage.ru_stime.tv_usec - t->usage.ru_stime.tv_usec) / 1e6;  // Calculate system CPU time
    
    print_pid_cpu();
    printf("real time: %.4f seconds  ", elapsed);  // Log the elapsed time
    printf("user: %.4f seconds  ", user_time);  // Log user CPU time
    printf("sys: %.4f seconds  \n", sys_time);  // Log system
}

void print_pid_cpu_operation(char *operation) {
    print_pid_cpu();
    printf("%s\n", operation);
}



int main(int argc, char *argv[]) {
    struct profile_times t;
    char *operation = malloc(256);
    // TODO profile doing a bunch of floating point muls
    float x = 1.0;
    sprintf(operation, "%d fmuls", NUM_MULS);
    print_pid_cpu_operation(operation);
    profile_start(&t);
    for (int i = 0; i < NUM_MULS; i++)
        x *= 1.1;
    profile_log(&t);

    // TODO profile doing a bunch of mallocs
    sprintf(operation, "%d mallocs of size %d", NUM_MALLOCS, MALLOC_SIZE);
    print_pid_cpu_operation(operation);
    profile_start(&t);
    void *p;
    for (int i = 0; i < NUM_MALLOCS; i++)
    p = malloc(MALLOC_SIZE);
    profile_log(&t);

    // TODO profile sleeping
    sprintf(operation, "sleeping for %d", SLEEP_SEC);
    print_pid_cpu_operation(operation);
    profile_start(&t);
    sleep(SLEEP_SEC);
    profile_log(&t);

    free(operation);
    return 0;
}
