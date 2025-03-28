#include "header.hpp"
using std::cout; using std::endl;

int main() {
    int fd = shm_open("/sharedmemry", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        cout << "Error with shm_open: " << errno << endl;
    }
    struct shared *mem;

    int err = ftruncate(fd, sizeof(*mem));
    if(err == -1) { 
        cout << "Error with ftruncate: " << errno << endl;
    }

    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(mem == MAP_FAILED) {
        cout << "Error with mmap: " << errno << endl;
    }
    close(fd);

    sem_init(&(mem->semaphore), 1, 1);

    srand(time(nullptr));
 
    for(int i = 0; i < 25; ++i) {
        sem_wait(&(mem->semaphore));

        // Producer produces values from 1 to 100
        // A value of 0 represents NOTHING and is not
        // a valid value for consumer to take
        int value = (rand() % 100) + 1;

        // Randomly selects which slot to check first
        int prod_first = rand() % 2;
        int prod_second = prod_first == 0 ? 1 : 0;
            
        if(mem->arr[prod_first] == 0) {
            cout << "\033[34mProducer made: \t" << value << endl;
            mem->arr[prod_first] = value;
        } else if (mem->arr[prod_second] == 0) {
            cout << "\033[34mProducer made: \t" << value <<  endl;
            mem->arr[prod_second] = value;
        }

        sem_post(&(mem->semaphore));
        sleep(rand() % 2);
    }
    sem_destroy(&(mem->semaphore));

    cout << "\033[97m";

    fd = shm_unlink("/sharedmemry");
    if(fd == -1) {
        cout << "Error with shm_unlink: " << errno << endl;
    }

    exit(EXIT_SUCCESS);
}