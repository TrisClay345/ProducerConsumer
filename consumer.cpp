#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/sharedmemry", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        cout << "Error with shm_open: " << errno << endl;
    }
    struct shared* mem;

    int err = ftruncate(fd, sizeof(*mem));
    if(err == -1) { 
        cout << "Error with ftruncate: " << errno << endl;
    }

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(mem == MAP_FAILED) {
        cout << "Error with mmap: " << errno << endl;
    }
    close(fd);

    // Ensures that consumer enters after producer.
    // Stops deadlocking bug.
    sleep(2);

    for(int i = 0; i < 25; ++i) {
        sem_wait(&(mem->semaphore));

        // Randomly selects which slot to check first
        int check_first = rand() % 2;
        int check_second = check_first == 0 ? 1 : 0;

        if(mem->arr[check_first] != 0) {
            cout << "\033[31mConsumer took: \t" << mem->arr[check_first] << endl;
            mem->arr[check_first] = 0;
        } else if (mem->arr[check_second] != 0) {
            cout << "\033[31mConsumer took: \t" << mem->arr[check_second] << endl;
            mem->arr[check_second] = 0;
        }

        sem_post(&(mem->semaphore));
        sleep(rand() % 2);
    }
    exit(EXIT_SUCCESS);
}