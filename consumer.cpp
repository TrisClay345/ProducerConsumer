#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/shared_memory", O_RDWR, 0);
    struct shared* mem;

    ftruncate(fd, sizeof(*mem));

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    int i = 0;
    while(i < 10'000) {
        sem_wait(&(mem->full));
        sem_wait(&(mem->mutex));

        int value;
        value = mem->arr[mem->out];
        cout << "Consumer took: " << value << endl;

        mem->out = (mem->out + 1) % 2;

        sem_post(&(mem->mutex));
        sem_post(&(mem->empty));
        ++i;
    }
    shm_unlink("/shared_memory");
}