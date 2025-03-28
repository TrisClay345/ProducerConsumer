#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/sharedmemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        cout << "Error with shm_open: " << errno << endl;
    }
    struct shared* mem;

    ftruncate(fd, sizeof(*mem));

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(mem == MAP_FAILED) {
        cout << "Error with mmap: " << errno << endl;
    }
    close(fd);
    
    for(int i = 0; i < 50; ++i) {
        sem_wait(&(mem->full));
        sem_wait(&(mem->mutex));

        int value;
        value = mem->arr[mem->out];
        cout << "Consumer took: \t\t" << value << endl;

        mem->out = (mem->out + 1) % 2;

        sem_post(&(mem->mutex));
        sem_post(&(mem->empty));
    }
    sem_destroy(&(mem->full));
    sem_destroy(&(mem->empty));
    sem_destroy(&(mem->mutex));

    fd = shm_unlink("/sharedmemory");
    if(fd == -1) {
        cout << "Error with shm_unlink: " << errno << endl;
    }
}