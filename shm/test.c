#include "shm.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

struct student {
    int age;
    char name[32];
};

int main () {
    struct shm* shm = shmInit("./",0x01,4096);
    struct student tmp;
    
    if (fork()==0) {
        scanf("%s %d",tmp.name,&tmp.age);
        memcpy(shm->ptr,&tmp,sizeof(tmp));
        return 0;
    }
    wait(NULL);

    struct student tmp2;
    memcpy(&tmp2,shm->ptr,sizeof(tmp2));
    printf("%s %d\n",tmp2.name,tmp2.age);

    shm->remove(shm);
}