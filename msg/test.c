#include "msg.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

struct student {
    int age;
    char name[32];
};

int main () {
    struct msg* msg = msgInit("./",0x01);

    struct student tmp;
    
    if (fork()==0) {
        scanf("%s %d",tmp.name,&tmp.age);
        msg->send(msg->id,&tmp,sizeof(tmp),114514);
        return 0;
    }

    struct student tmp2;
    msg->receive(msg->id,&tmp2,sizeof(tmp2),114514);
    printf ("%s %d\n",tmp2.name,tmp2.age);

    wait(NULL);
    msg->remove(msg);
}