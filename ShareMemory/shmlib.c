#include "shmlib.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

shm_t* shm_initialize(int shmkey) {
    int shmid = shmget(shmkey, sizeof(shm_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    shm_t* shm = shmat(shmid, NULL, 0);
    if (shm == (void*) -1) {
        perror("shmat failed");
        exit(1);
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shm->mtx, &attr);

    return shm;
}

void shm_finalize(int shmid, shm_t* shm) {
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
}

void update_data(shm_t* shm, int value) {
    pthread_mutex_lock(&shm->mtx);
    shm->data = value;
    shm->r = 0;
    pthread_mutex_unlock(&shm->mtx);
}

int read_data(shm_t* shm, int* r) {
    pthread_mutex_lock(&shm->mtx);
    *r = shm->r;
    int data = shm->data;
    pthread_mutex_unlock(&shm->mtx);
    return data;
}