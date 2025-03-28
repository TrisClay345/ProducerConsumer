#ifndef HEADER_CPP
#define HEADER_CPP

#include <iostream>
#include <semaphore.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

struct shared {
    int arr[2];
    sem_t semaphore;
};

#endif