#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

typedef struct shm_struct_for_test
{
    pthread_mutex_t mtx;    // 공유메모리 동기화 용 뮤텍스
    int r;      // read index
    int data;   // data for test
}shm_t;         // shared memory struct

shm_t *g_shm;

int main(int argc, char** argv)
{
    int shmid = 0;
    int shmkey = 0x12345678;
    pthread_mutexattr_t attr;   // thread attribute

    if( (shmid = shmget(shmkey, (size_t)sizeof(shm_t), IPC_CREAT | 0666)) < 0 ) // shared memory 생성
    {
        printf("shmget error!\n");
        return -1;
    }
    printf("shimd = %d\n", shmid);

    if( (g_shm = shmat(shmid, 0, 0)) < 0 )  // shared memory attach
    {
        printf("shmat error!\n");
        return -1;
    }

    // 스레드 Attribute 초기화
    if( pthread_mutexattr_init(&attr) )
    {
        printf("init mutexattr error!\n");
        return -1;
    }
    // 스레드를 프로세스 간 공유하도록 속성 세팅
    if( pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) )
    {
        printf("setpshared mutexattr error!\n");
        return -1;
    }

    // 뮤텍스 초기화 - 위에서 설정한 Attribute 적용
    if( pthread_mutex_init(&g_shm->mtx, &attr) )
    {
        printf("init mutex error!\n");
        return -1;
    }
    g_shm->data=1000;
    while(1)
    {

        g_shm->r = 0;
        g_shm->data++;
        printf("P1 [%d] data = %d\n", g_shm->r, g_shm->data);
        sleep(5);
        
    }

    return 1;
}