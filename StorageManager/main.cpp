#include "StorageManager.h"

int main(int argc, char** argv) {

    int total = 0;
    int ram = 1;
    StorageManager sManager;

    //if no overgraded parameters have been passed 

/*
    while (!(ram < total)) {
        std::cout << "WILKOMMEN!" << std::endl;
        std::cout << "Anzahl total:";
        std::cin >> total;
        std::cout << "\nAnzahl ram:";
        std::cin >> ram;
    }
*/

    sManager.init_store(10, 5);

    sManager.cleanup_store();

    sManager.init_store(10, 5);


    return 0;
}

