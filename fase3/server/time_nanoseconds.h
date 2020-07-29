#ifndef _TIME_NANOSECONDS_H_
#define _TIME_NANOSECONDS_H_

static int current_time_nanoseconds(){
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

static int randomNumber(){
    srand(current_time_nanoseconds() * 1000000000);
    return rand();
}

#endif
