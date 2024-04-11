#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

typedef struct shm_struct_for_test
{
    pthread_mutex_t mtx;
    int r;
    int data;
}shm_t;

shm_t *g_shm;

int main(int argc, char** argv)
{
    int shmid = 0;
    int shmkey = 0x12345678;

    if( (shmid = shmget(shmkey, (size_t)sizeof(shm_t), IPC_CREAT | 0666)) < 0 )
    {
        printf("shmget error!\n");
        return -1;
    }
    printf("shimd = %d\n", shmid);

    if( (g_shm = shmat(shmid, 0, 0))< 0 )
    {
        printf("shmat error!\n");
        return -1;
    }

    // 공유메모리에 써진 데이터를 읽는 프로세스 중 하나
    while(1)
    {
        // r값 = 0 이면, 두 프로세스 모두 읽지 않음
        // r값 = 1 이면, P2만 읽음
        // r값 = 2 이면, P3만 읽음
        // r값 = 3 이면, 두 프로세스 모두 읽음
        if(g_shm->r != 1 && g_shm->r != 3)
        {
            usleep(100);
            pthread_mutex_lock(&g_shm->mtx);    // 뮤텍스 잠금 - 데이터 읽을때 다른 프로세스(P3)는 접근하지 못함
            printf("P2 [%d] Start data = %d P2\n", g_shm->r, g_shm->data);
            sleep(1);   // 데이터 처리 하는 시간을 1초로 시뮬레이션

            // 데이터 처리 끝났으면
            if(g_shm->r != 2)   // P3가 작업 안 끝났으면
                g_shm->r = 1;   // P2만 작업 끝난 것으로
            else                // P3도 작업 끝난 상태라면
                g_shm->r = 3;   // 모두 작업 끝난 것으로 플래그 세팅
            printf("P2 [%d] Finish data = %d P2\n", g_shm->r, g_shm->data);
            pthread_mutex_unlock(&g_shm->mtx);  // 뮤텍스 잠금 해제
        }
    }
    return 1;
}