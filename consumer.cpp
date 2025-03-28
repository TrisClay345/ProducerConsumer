/*
    Tristian Clayman
    consumer.cpp - consumer program for producer-consumer problem
    3/28/2025
*/

#include "header.hpp"
using std::cout; using std::endl;

int main() {
    /*
        Creates or opens shared memory. Prints if there's an error.
    */
    int fd = shm_open("/sharedmemry", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        cout << "Error with shm_open: " << errno << endl;
    }

    /*
        Resizes shared memory to size of structure that I wish to put there.
        Prints when an error is encountered.
    */
    struct shared* mem;
    int err = ftruncate(fd, sizeof(*mem));
    if(err == -1) { 
        cout << "Error with ftruncate: " << errno << endl;
    }

    /*
        Get the memory location in an easily modifiable state using memory map function.
        Also prints if an error is encountered.
    */
    mem = static_cast<shared*>(mmap(NULL, sizeof(*mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(mem == MAP_FAILED) {
        cout << "Error with mmap: " << errno << endl;
    }
    close(fd);

    // Ensures that consumer enters after producer.
    // Stops deadlocking bug.
    sleep(2);

    /*
        This loop is where the 'consuming' happens. It decides which slot to check first randomly,
        and takes in that order if it can. If both slots are empty, it does nothing.
        Uses the semaphore to ensure mutual exclusivity.
    */
    for(int i = 0; i < 25; ++i) {
        // Program will wait if other program is in its critical section
        sem_wait(&(mem->semaphore));

        // Randomly selects which slot to check first
        int check_first = rand() % 2;
        int check_second = check_first == 0 ? 1 : 0;

        // Checks the first choice then second choice. If neither have an item, does nothing.
        if(mem->arr[check_first] != 0) {
            cout << "\033[31mConsumer took: \t" << mem->arr[check_first] << endl;
            mem->arr[check_first] = 0;
        } else if (mem->arr[check_second] != 0) {
            cout << "\033[31mConsumer took: \t" << mem->arr[check_second] << endl;
            mem->arr[check_second] = 0;
        }

        // Semaphore signals to waiting programs there are no programs in their critical section.
        sem_post(&(mem->semaphore));

        //Will wait for a second 50% of the time.
        sleep(rand() % 2);
    }
    exit(EXIT_SUCCESS);
}