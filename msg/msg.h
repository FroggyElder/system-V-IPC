#ifndef _MSG_H
#define _MSG_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DEBUG_INFO printf ("[%s:%d] %s\n",__func__,__LINE__,__FILE__)

struct msg_buf {
    long type;
    char data[1024];
};

struct msg {
    int id;

    bool (* send) (int msg_id,void const* src,size_t size,long type);
    bool (* receive) (int msg_id,void* dest,size_t size,long type);
    bool (* remove) (struct msg* msg);
};

struct msg* msgInit (char* pathname,int proj_id);

static bool __msgSnd (int msg_id,void const* src,size_t size,long type);

static bool __msgRcv (int msg_id,void* dest,size_t size,long type);

static bool __msgRm (struct msg* msg);

#endif