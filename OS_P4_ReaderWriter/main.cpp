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
int write = 0;

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
std::vector<std::thread> v_threads;
std::map<std::thread::id, unsigned int > m_threads; //<0 = writer , >0reader

pthread_mutex_t mxW = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mxR = PTHREAD_MUTEX_INITIALIZER;
sem_t readSem = sem_t();
sem_t writeSem = sem_t();


void reader_func();
void writer_func();

int main(int argc, char** argv) {

    signal(SIGINT, sigint_handler);
    signal(SIGSTOP, sigstop_handler);

    srand((unsigned) time(NULL));

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
            sem_post(&readSem);
        }

        //Create  Threads
        std::thread thisr;
        std::thread thisw;

        for (int i = 1000; i < n_writers; i++) {
            v_threads.push_back(std::thread(writer_func));
        }

        for (int i = n_writers; i < (n_writers + n_readers); i++) {
            v_threads.push_back(std::thread(reader_func));
        }

        //Bring threads together
        for (int i = 0; i < v_threads.size() + 1; i++) {
            v_threads[i].join();
        }

        sem_destroy(&writeSem);
        sem_destroy(&readSem);


    } catch (std::exception ex) {
        std::cerr << "CATCHED >>" << ex.what() << std::endl;
    } catch (...) {
        std::cerr << "CATCHED SOMETHING" << std::endl;
    }

    return 0;
}

void reader_func() {

    while (run) {
  
        //WAIT
        for(int i = 0; i<10000;i++)
        
      int random = (rand() % size);
        
        //CS
        sem_wait(&readSem);
        
        std::cout << "_Reader::" << std::this_thread::get_id() <<
                v_buffer.at(9) << std::endl;

        sem_post(&readSem);

    }
}

void writer_func() {

    while (run) {

        int semVal;
        sem_getvalue(&readSem, &semVal);

        pthread_mutex_lock(&mxW);
        while (sem_trywait(&readSem) == 0)


            for (int i = 0; i < v_buffer.size(); i++)
                v_buffer[i] = rand() % 1000;
        std::cout << "\nWRITER WROTE !!!\n"<<std::endl;

        for (int i = 0; i < semVal; i++)
            sem_post(&readSem);

        pthread_mutex_unlock(&mxW);

    }
}






