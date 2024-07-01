#ifndef _SHM_H
#define _SHM_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_INFO printf("[%s:%d] %s\n",__func__,__LINE__,__FILE__)

struct shm {
    int id;
    void* ptr;

    bool (* remove)(struct shm* shm);
};

struct shm* shmInit(char* pathname,int proj_id,size_t size);

static bool __shmRm(struct shm* shm);

#endif