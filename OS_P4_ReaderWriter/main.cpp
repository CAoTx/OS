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
std::vector<std::thread> v_threads;

//pthread_mutex_t mxW = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mxR = PTHREAD_MUTEX_INITIALIZER;
sem_t readSem = sem_t();
sem_t writeSem = sem_t();


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
        std::cout << "Größen Festlegen:" << std::endl;
        std::cout << "\n1.BufferSize:";
        std::cin>>size;
        std::cout << "2.ReaderAmount:";
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
            sem_post(&readSem);
        }

        //Create  Threads
        std::thread thisr;
        std::thread thisw;

        for (int i = 1; i < n_readers + 1; i++) {
            thisr = std::thread(reader_func);
            m_threads.insert(std::pair<std::thread::id, unsigned int>(thisr.get_id(), i));
        }

        for (int i = 1000; i < (n_writers + 1000); i++) {
            thisw = std::thread(writer_func);
            m_threads.insert(std::pair< std::thread::id, unsigned int>(thisw.get_id(), i));
        }
    } catch (std::exception ex) {
        std::cerr << "CATCHED >>" << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "CATCHED SOMETHING" << std::endl;
    }

    run = false;
    return 0;
}

void reader_func() {

    while (run) {
        int pos = rand() % size;
        std::thread::id tid = std::this_thread::get_id();

        //ENTER
        sem_wait(&readSem);
        sem_wait(&writeSem);
        //OUTPUT
        std::cout << "_ReaderID:" << m_threads[tid] << " - Zahl:"
                << v_buffer[pos] << " - PID:" << tid << std::endl;
        //EXIT
        sem_post(&writeSem);
        sem_post(&readSem);
    }
}

void writer_func() {

    while (run) {
        //        int semValue;
        //        sem_getvalue(&reamSem, &semValue);
        //
        //        while (semValue > 0) {
        //            sem_trywait(&reamSem);
        //            sem_getvalue(&reamSem, &semValue);
        //        }

        //wait()
        for (int i = 0; i < 10000; i++) {
            ;
        }

        sem_wait(&writeSem);

        for (int i = 0; i < size; i++) {
            v_buffer[i] = rand() % 100;
        }
        std::cout << "WRITER WROTE" << std::endl;

        sem_post(&writeSem);


        //        sem_post(&reamSem);
        //        for (int i = 0; i < n_readers; i++) {
        //            sem_post(&reamSem);
        //        }
    }
}






