# ProducerConsumer
Producer-consumer Problem
Tristian M. Clayman
3/28/2025

This project was to make two separate processes use shared memory. It uses
POSIX threads and semaphores to do so, namely the shm_open, mmap, and sem_open functions.

producer.cpp and consumer.cpp share memory. This memory contains an array of two integers
(representing the table with two spaces), and the semaphore for the critical section problem.

producer.cpp produces a random integer from one to one-hundred and places it randomly into one of the
open spaces on the table. NOTE: zero is not a valid value and instead represents that nothing is on the table.

consumer.cpp takes randomly from the table if it can.

Both files print their progress on the command line, with producer in blue and consumer in red.

TO RUN:
This program comes with a Makefile. Simply type 'make' in the command line and everything will be done for you.

'make rm' also works and deletes the application files created by 'make'.

If you really want to compile it yourself, I used 'g++' with the flags '-pthread -lrt' and '-o producer' or '-o consumer'.
Once you have both executables, run them with './producer & ./consumer &'.

runs on Linux OS, untested on other platforms.
Made using Github codespaces.
