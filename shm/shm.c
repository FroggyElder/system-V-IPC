#include "shm.h"

struct shm* shmInit(char* pathname,int proj_id,size_t size) {
    int key = ftok(pathname,proj_id);
    if (key<0) {
        DEBUG_INFO;
        perror("");
        return NULL;
    }
    int shm_id = shmget(key,size,IPC_CREAT|0666);
    if (shm_id<0) {
        DEBUG_INFO;
        perror("");
        return NULL;
    }
    
    struct shm* new_shm = (struct shm*)malloc(sizeof(struct shm));
    new_shm->id = shm_id;
    new_shm->ptr = shmat(new_shm->id,NULL,0);
    new_shm->remove = __shmRm;

    return new_shm;
}

static bool __shmRm(struct shm* shm) {
    if (shm==NULL) {
        DEBUG_INFO;
        return false;
    }
    if (shmdt(shm->ptr)<0) {
        DEBUG_INFO;
        perror("");
        return false;
    }
    if (shmctl(shm->id,IPC_RMID,NULL)<0) {
        DEBUG_INFO;
        perror("");
        return false;
    }

    free(shm);
    return true;
}