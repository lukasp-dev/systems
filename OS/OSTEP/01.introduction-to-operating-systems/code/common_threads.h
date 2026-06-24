#ifndef __common_threads_h__
#define __common_threads_h__

#include <pthread.h>
#include <assert.h>

void Pthread_create(pthread_t *t, const pthread_attr_t *attr,
                    void *(*start_routine)(void *), void *arg) {
    int rc = pthread_create(t, attr, start_routine, arg);
    assert(rc == 0);
}

void Pthread_join(pthread_t thread, void **value_ptr) {
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}

#endif // __common_threads_h__
