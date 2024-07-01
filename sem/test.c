#include "sem.h"

int main () {
    int set[] = {1,2,3,4,5};
    struct sem* sem = semInit("./",0x01,5,set);

    sem->remove(sem);
}