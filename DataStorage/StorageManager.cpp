#include "StorageManager.h"

StorageManager::StorageManager() {
    this->homePath = "./";
}

StorageManager::StorageManager(std::string homepath) : homePath(homepath) {
};
StorageManager::StorageManager(std::string homepath, int buffersize) 
: homePath(homepath), bufferSize(buffersize) {
};

StorageManager::StorageManager(const StorageManager& orig) {
}

StorageManager::~StorageManager() {
}

int StorageManager::newId() {
    blockIdCtr++;
    return blockIdCtr;
}

void StorageManager::init_store(int total_blocks, int ram_blocks) {

    //Abort possibilities 
    if (total_blocks < ram_blocks){
        std::cout<<"init_store: more Ram than TotalBlocks";
        return;
    }
    
    buffer.resize(bufferSize, ' ');
    //First create Blocks on Storage
    for (int i = 0; i < total_blocks; i++) {
        int id = newId();
        writeS.open(homePath + std::to_string(id));
       // writeS << buffer; versteh ich nicht
        writeS.close();
        totalBlocks.push_back(id);
    }

    //Then load the specific amount of blocks into the cache LRU 
    //Already used
    for (int j = 0; j < ram_blocks; j++) {
        int id = totalBlocks.at(total_blocks - j);
        Block newy; 
        newy.id = id;
        readS.open(homePath + std::to_string(id));
        readS>>newy.data;
        readS.close();
        holdedBlocks.insert(std::pair<Block, bool> (newy, true));
    }
    return;
}




