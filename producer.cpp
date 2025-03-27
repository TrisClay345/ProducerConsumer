#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Set up shared memory
    int flags = O_CREAT | O_RDWR;
    int mode = S_IRUSR | S_IWUSR;
    int fd = shm_open("/shrdmemry", flags, mode);
    if(fd == -1) {
        std::cout << "An error occured with the shm_open function." << std::endl;
        exit(-1);
    }
    ftruncate(fd, 8);

    // Start of critical section

    // End of critical section

    
    shm_unlink("/shrdmemry");
}   