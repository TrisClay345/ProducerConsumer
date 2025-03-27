#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

int main() {
    // Set up shared memory
    int flags = O_CREAT | O_RDWR;
    int mode = S_IRUSR | S_IWUSR;
    int fd = shm_open("/shrdmemry", flags, mode);
    if(fd == -1) {
        std::cout << "An error occured with the shm_open function." << std::endl;
        exit(1);
    }
    // Resize shared memory
    int err = ftruncate(fd, 128);
    if(err == -1) {
        std::cout << "An error occured with the ftruncate function." << std::endl;
        exit(1);
    }
    
    // Get the memory in a read/writeable state
    int prot = PROT_READ | PROT_WRITE;
    void* tmp = mmap(NULL, 8, prot, MAP_SHARED, fd, 0);
    if(tmp == MAP_FAILED) {
        std::cout << "An error occured with the mmap function." << std::endl;
        exit(1);
    }
    int* mem = static_cast<int*>(tmp);

    // Create shared semaphore
    sem_t* semaphore;
    semaphore = sem_open("/mysemaphore", flags, mode, 1); 
    if(semaphore == SEM_FAILED) {
        std::cout << "An error occured with the sem_open function." << std::endl;
        exit(1);
    }

    srand(time(nullptr) % 1772799);
    mem[0] = -1;
    mem[1] = -1;
    int counter = 10'000;
    while(true) {
        sem_wait(semaphore);
        // Start of critical section
        if(mem[0] != -1) {
            std::cout << mem[0] << std::endl;
            mem[0] = -1;
        } else if(mem[1] != -1) {
            std::cout << mem[0] << std::endl;
            mem[0] = -1;
        }
        // End of critical section
        sem_post(semaphore);
    }
    sem_unlink("/mysemaphore");
    shm_unlink("/shrdmemry");
}   