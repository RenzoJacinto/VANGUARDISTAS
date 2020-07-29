#ifndef _TIME_NANOSECONDS_H_
#define _TIME_NANOSECONDS_H_

int current_time_nanoseconds(){
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    return tm.tv_nsec;
}

#endif
