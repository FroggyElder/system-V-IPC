#include "sem.h"

struct sem* semInit (char* pathname,int proj_id,int num_sems,int* init_nums) {
    key_t key = ftok(pathname,proj_id);
    if (key<0) {
        DEBUG_INFO;
        perror ("");
        return NULL;
    }
    int sem_id = semget(key,num_sems,IPC_CREAT|0666);
    if (sem_id<0) {
        DEBUG_INFO;
        perror ("");
        return NULL;
    }

    struct sem* sem = (struct sem*)malloc(sizeof(struct sem));
    sem->id = sem_id;
    sem->nums = num_sems;

    sem->remove = __semRm;
    sem->op = __semOp;

    if (init_nums) {
        for (int i=0;i<sem->nums;i++) {
            semctl(sem->id,i,SETVAL,init_nums[i]);
        }
    }
    else semctl(sem->id,0,SETALL,0);

    return sem;
}

static bool __semOp (struct sem* sem,int* op_vals) {
    struct sembuf* ops = (struct sembuf*)malloc(sizeof(struct sembuf)*sem->nums);
    if (ops==NULL) {
        DEBUG_INFO;
        perror("");
        return false;
    }

    int op_nums = 0;

    for (int i=0;i<sem->nums;i++) {
        if (op_vals[i]>0) {
            ops[op_nums].sem_flg = SEM_UNDO;
            ops[op_nums].sem_op=op_vals[i];
            ops[op_nums++].sem_num=i;
        }
    }
    if(semop(sem->id,ops,op_nums)<0) {
        DEBUG_INFO;
        perror("");
        goto semop_error;
    }

    free(ops);
    return true;

semop_error:
    free(ops);
    return false;
}

static bool __semRm (struct sem* sem) {
    if (sem==NULL) {
        DEBUG_INFO;
        return false;
    }
    if (semctl(sem->id,0,IPC_RMID)<0) {
        DEBUG_INFO;
        return false;
    }

    free(sem);
    return true;
}