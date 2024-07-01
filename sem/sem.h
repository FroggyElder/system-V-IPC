#ifndef _SEM_H
#define _SEM_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG_INFO printf ("[%s:%d] %s\n",__func__,__LINE__,__FILE__)

struct sem {
    int id;
    int nums;

    bool (* remove) (struct sem* sem);
    bool (* op) (struct sem* sem, int* op_nums);
};

/// @brief Init a sem Id
/// @param pathname a path to specify key
/// @param proj_id a id to specify key
/// @param num_sems the size of sem nums
/// @param init_nums the array to init with, set as NULL for all zero
/// @return the struct initiated
struct sem* semInit (char* pathname,int proj_id,int num_sems,int* init_nums);

static bool __semOp (struct sem* sem,int* op_nums);

static bool __semRm (struct sem* sem);

#endif