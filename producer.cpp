/*
    Tristian Clayman
    producer.cpp - producer program for producer-consumer problem.
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
    struct shared *mem;
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
    /*
        Close old file descriptor since it is no longer needed.
    */
    close(fd);

    /*
        Initialize the semaphore in the shared memory.
    */
    sem_init(&(mem->semaphore), 1, 1);


    /*
        This loop is where the meat of the 'producer' lies. It decides the value to produce,
        where to put it, and uses the semaphore to ensure proper synchronization between
        the two programs. 
    */
    srand(time(nullptr));
 
    for(int i = 0; i < 25; ++i) {
        // Program will wait until other program leaves its critical selection.
        sem_wait(&(mem->semaphore));

        // Producer produces values from 1 to 100
        // A value of 0 represents NOTHING and is not
        // a valid value for consumer to take
        int value = (rand() % 100) + 1;

        // Randomly selects which slot to check first
        int prod_first = rand() % 2;
        int prod_second = prod_first == 0 ? 1 : 0;
            
        // Puts the value in the first choice if it can,
        // puts it in second choice if it needs to.
        // Does nothing if both are full.
        if(mem->arr[prod_first] == 0) {
            cout << "\033[34mProducer made: \t" << value << endl;
            mem->arr[prod_first] = value;
        } else if (mem->arr[prod_second] == 0) {
            cout << "\033[34mProducer made: \t" << value <<  endl;
            mem->arr[prod_second] = value;
        }

        // Semaphore tells waiting programs that no one is in their critical section 
        sem_post(&(mem->semaphore));
        
        // Will wait for a second 50% of the time. 
        sleep(rand() % 2);
    }
    /*
        Destroy shared semaphore.
    */
    sem_destroy(&(mem->semaphore));

    //This just changes the color of the console text back to bright white.
    cout << "\033[97m";

    /*
        Unlink shared memory. This deallocates (deletes) everything that was held there.
        Prints an error if one is encountered.
    */
    fd = shm_unlink("/sharedmemry");
    if(fd == -1) {
        cout << "Error with shm_unlink: " << errno << endl;
    }

    exit(EXIT_SUCCESS);
}