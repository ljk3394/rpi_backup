#ifndef SHMLIB_H
#define SHMLIB_H

#include <pthread.h>

typedef struct shm_struct_for_test {
    pthread_mutex_t mtx;
    int r;
    int data;
} shm_t;

shm_t* shm_initialize(int shmkey);
void shm_finalize(int shmid, shm_t* shm);
void update_data(shm_t* shm, int value);
int read_data(shm_t* shm, int* r);

#endif