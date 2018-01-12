#include "StorageManager.h"

int main(int argc, char** argv) {

    int total, ram;
    StorageManager sManager;

    //if no overgraded parameters have been passed 
    if (argc = nullptr && argv[1] == nullptr) {

        while (ram >= total) {
            std::cout << "WILKOMMEN!" << std::endl;
            std::cout << "Anzahl total:";
            std::cin >> total;
            std::cout << "\nAnzahl ram:";
            std::cin >> ram;
        }
    }
    
    sManager.init_store(total, ram);
    
    



    return 0;
}

