#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/shared_memory", O_CREAT | O_EXCL | O_RDWR, 0600);
    struct shared *mem;
    ftruncate(fd, sizeof(*mem));

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    sem_init(&(mem->mutex), 1, 1);
    sem_init(&(mem->full), 1, 0);
    sem_init(&(mem->empty), 1, 2);

    srand(time(nullptr));
    mem->in = 0; mem->out = 0;
    int i = 0;
    while(i < 10'000) {
        sem_wait(&(mem->empty));
        sem_wait(&(mem->mutex));

        int value = rand();

        mem->arr[mem->in] = value;

        mem->in = ++(mem->in) % 2;

        sem_post(&(mem->mutex));
        sem_post(&(mem->full));
        ++i;
    }
    shm_unlink("/shared_memory");
}