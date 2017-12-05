#include <vector>
#include <iostream>
#include <map>
#include <time.h>
#include <signal.h>
#include <wait.h>
#include <exception>

#include <thread>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <mutex>
#include <cstdlib>
#include <sys/sem.h>
#include <sys/ipc.h>


#define N  250

bool run = true;

void sigint_handler(int signum) {
    std::cerr << "Pressed C" << signum << std::endl;
    run = false;
}

void sigstop_handler(int signum) {
    std::cerr << "Pressed Z: " << signum << std::endl;
    run = false;
}

int size, n_readers, n_writers;
std::vector<int> v_buffer;
std::map<std::thread::id, unsigned int > m_threads; //<0 = writer , >0reader

//pthread_mutex_t mxW = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mxR = PTHREAD_MUTEX_INITIALIZER;
sem_t sem = sem_t();


void reader_func();
void writer_func();

int main(int argc, char** argv) {

    signal(SIGINT, sigint_handler);
    signal(SIGSTOP, sigstop_handler);

    srand((unsigned) time(NULL));

    //size = sem_init(&sem, 0, N); //muss in main ... -.-

    //Thanks sebCheff   
    if (argc == 4) {
        size = std::stoi(argv[1]);
        n_readers = std::stoi(argv[2]);
        n_writers = std::stoi(argv[3]);
    } else {
        std::cout << "Größen Festlegen:\n1.BufferSize:";
        std::cin>>size;
        std::cout << "\n2.ReaderAmount:";
        std::cin >>n_readers;
        std::cout << "3.WriterAmount:";
        std::cin>>n_writers;
        std::cout << std::endl;
    }
    try {
        //Fill up vector
        for (int i = 0; i < size; i++) {
            v_buffer.push_back(rand() % 1000);
            //  v_buffer.push_back(rand() % n_readers);
            sem_post(&sem);
        }
        //Create reader Threads
        for (int i = 1; i < n_readers + 1; i++) {
            m_threads.insert(std::pair<std::thread::id, unsigned int>(std::thread(reader_func).get_id(), i));
        }
        //Create writer Threads
        for (int i = -1; (-1 * i) < n_writers; i--) {
            m_threads.insert(std::pair< std::thread::id, unsigned int>(std::thread(writer_func).get_id(), i));
        }
    } catch (std::exception ex) {
        std::cerr << "CATCHED >>" << ex.what() << std::endl;
    }    catch (...) {
        std::cerr << "CATCHED SOMETHING" << std::endl;
    }

    run = false;
    return 0;
}

void reader_func() {

    while (run) {
        int pos = rand() % size;
        std::thread::id tid = std::this_thread::get_id();

        sem_wait(&sem);
        //OUTPUT
        std::cout << "_ReaderID:" << m_threads[tid] << " - PID:" << tid << " - Zahl:" << v_buffer[pos] << std::endl;
        sem_wait(&sem);
    }
    return;
}

void writer_func() {

    while (run) {
        int semValue;
        sem_getvalue(&sem, &semValue);

        //        for (int i = semValue; i > 1; i--) {
        //            sem_wait(&sem);
        //        }
        sem_wait(&sem); //Block Reader


        for (int i = 0; i < size; i++) {
            v_buffer[i] = rand() % 100;
        }
        std::cout << "WRITER WROTE" << std::endl;
        sem_post(&sem);
    }
    return;
}






