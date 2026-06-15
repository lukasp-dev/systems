#ifndef __common_h__
#define __common_h__

/* Used by figure2.1.c — see ../03.figure2.1_cpu_process_demo.md */

#include <sys/time.h>
#include <assert.h>

double GetTime() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}

void Spin(int howlong) {
    double t = GetTime();
    while (GetTime() - t < (double) howlong)
        ; // 아무것도 안 하고 기다림
}

#endif // __common_h__