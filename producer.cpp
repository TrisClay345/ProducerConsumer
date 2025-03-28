#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/sharedmemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        cout << "Error with shm_open: " << errno << endl;
    }
    struct shared *mem;
    ftruncate(fd, sizeof(*mem));

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(mem == MAP_FAILED) {
        cout << "Error with mmap: " << errno << endl;
    }
    close(fd);

    sem_init(&(mem->mutex), 1, 1);
    sem_init(&(mem->full), 1, 0);
    sem_init(&(mem->empty), 1, 2);

    srand(time(nullptr));
    mem->in = 0; mem->out = 0;
    
    for(int i = 0; i < 50; ++i) {
        sem_wait(&(mem->empty));
        sem_wait(&(mem->mutex));

        int value = (rand() % 100) + 1;
        cout << "Producer produced: \t" << value << endl;

        mem->arr[mem->in] = value;

        mem->in = ++(mem->in) % 2;

        sem_post(&(mem->mutex));
        sem_post(&(mem->full));
        ++i;
    }
    sem_destroy(&(mem->full));
    sem_destroy(&(mem->empty));
    sem_destroy(&(mem->mutex));

    fd = shm_unlink("/sharedmemory");
    if(fd == -1) {
        cout << "Error with shm_unlink: " << errno << endl;
    }
}