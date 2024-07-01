#include "msg.h"

struct msg* msgInit (char* pathname,int proj_id) {
    key_t key = ftok(pathname,proj_id);
    if (key<0) {
        perror (__func__);
        return NULL;
    }
    int msg_id = msgget(key,IPC_CREAT|0666);
    if (msg_id<0) {
        perror(__func__);
        return NULL;
    }

    struct msg* msg = (struct msg*)malloc(sizeof(struct msg));
    msg->id = msg_id;
    msg->receive = __msgRcv;
    msg->send = __msgSnd;
    msg->remove = __msgRm;

    return msg;
}

static bool __msgSnd (int msg_id,void const* src,size_t size,long type) {
    struct msg_buf buffer = {.type = type};
    if(memcpy(buffer.data,src,size)==NULL) {
        perror(__func__);
        return false;
    }
    if(msgsnd(msg_id,&buffer,size,0)<0) {
        perror(__func__);
        return false;
    }

    return true;
}

static bool __msgRcv (int msg_id,void* dest,size_t size,long type) {
    struct msg_buf buffer;
    if(msgrcv(msg_id,&buffer,size,type,0)<=0) {
        perror (__func__);
        return false;
    }
    if(memcpy(dest,buffer.data,size)<=0) {
        perror (__func__);
        return false;
    }

    return true;
}

static bool __msgRm (struct msg* msg) {
    if(msgctl(msg->id,IPC_RMID,NULL)<0) {
        perror(__func__);
        return false;
    }

    free(msg);
    return true;
}