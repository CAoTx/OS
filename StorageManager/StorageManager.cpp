#include <math.h>
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
    cleanup_store();
}

int StorageManager::newBlockID() {
    blockIdCtr++;
    return blockIdCtr;
}

bool StorageManager::saveLRU() {
    auto it = holdedBlocks.begin();

    if ((*it).second.consistent == false) {
        std::string path = homePath + std::to_string((*it).second.id);
        writeS.open(path.c_str());
        if (!writeS.good())
            return false;
        writeS << (*it).second.data; //falscher operator?
    }

    holdedBlocks.erase(it); //is it really done?

    return true;
}

void StorageManager::init_store(int total_blocks, int ram_blocks) {
    //Abort possibilities 
    if (total_blocks < ram_blocks) {
        std::cout << "init_store: more Ram than TotalBlocks";
        return;
    }

    //First create Blocks on Storage
    buffer.resize(bufferSize, ' ');
    for (int i = 0; i < total_blocks; i++) {
        int id = newBlockID();
        writeS.open(homePath + std::to_string(id));
        // writeS << buffer; versteh ich nicht
        writeS.close();
        totalBlocks.push_back(id);
    }

    //load the specific amount of blocks into the cache LRU 
    //Already used
    for (int j = 0; j < ram_blocks; j++) {
        int id = totalBlocks.at((total_blocks - 1) - j);
        Block newy;
        newy.id = id;
        holdedBlocks.insert(std::pair<int, Block> (newy.lastused, newy));
        readS.open(homePath + std::to_string(id));
        readS >> newy.data;
        readS.close();

    }
    return;
}

void StorageManager::cleanup_store() {
    while (blockIdCtr > 0) {
        std::string path = (homePath + std::to_string(blockIdCtr));
        std::remove(path.c_str());
        blockIdCtr--;
    }
    this->blockIdCtr = 0;
    totalBlocks.clear();
    holdedBlocks.clear();
    buffer.resize(bufferSize, ' ');
}

int StorageManager::readCacheBlock(int blockID, std::string& buffer) {
    //due to extremely complex managing of all created and still existing Blocks, 
    // there is no real searching through the harddrive required


    for (std::map<int, Block>::iterator it = holdedBlocks.begin(); it != holdedBlocks.end(); it++) {
        if (it->first == blockID) {
            (*it).second.lastused = time(0);
            (*it).second.consistent = false;
            buffer = (*it).second.data;
            return 1;
        }
    }

    for (int j = 0; j < totalBlocks.size(); j++) {
        if (totalBlocks.at(j) == blockID) {
            Block newy;

            try {
                std::string path = homePath + std::to_string(blockID);
                readS.open(path.c_str());
                if (!readS.good())
                    throw;
                Block newy;
                readS>>buffer;
                newy.data = buffer;
                saveLRU();
                holdedBlocks.insert(std::pair<int, Block>(newy.lastused, newy));
            } catch (std::string text) {
                std::cout << "/nreadCacheBlock error, blockID:" << blockID << std::endl;
                std::cout << text << "/n" << std::endl;
                return -1;
            } catch (std::exception exc) {
                std::cout << "readCacheBlock error, blockID:" << blockID << std::endl;
                exc.what();
                return -1;
            }
        }
    }

    return 0;

}

int StorageManager::writeCacheBlock(int blockID, std::string buffer) {

    if (blockID == -1) {
        saveLRU();
        Block newy;
        newy.id = newBlockID();
        newy.data = buffer;
        writeS.open(homePath + std::to_string(newy.id));
        writeS << buffer;
        writeS.close();
        return newy.id;
    }

    for (auto it = holdedBlocks.begin(); it != holdedBlocks.end();) {
        if ((*it).second.id == blockID) {
            (*it).second.data = buffer;
            (*it).second.lastused = time(0);
            (*it).second.consistent = false;
            return blockID;
        }

        for (int i = 0; i < totalBlocks.size(); i++) {
            if (totalBlocks.at(i) == blockID) {
                saveLRU();
                Block newy;
                newy.id = blockID;
                newy.data = buffer;
                newy.consistent = false;
                holdedBlocks.insert(std::pair<int, Block>(newy.lastused, newy));
                return blockID;
            }
        }
        return -1;
    }
}

int StorageManager::freeCacheBlock(int blockID) {
    int flag = -1;

    for (auto it = holdedBlocks.begin(); it != holdedBlocks.end(); it++) {
        if ((*it).second.id == blockID) {
            holdedBlocks.erase(it);
            flag++;
        }
    }
    for (std::vector<int>::iterator jt = totalBlocks.begin(); jt != totalBlocks.end(); jt++) {
        if (*jt == blockID) {
            totalBlocks.erase(jt);
            flag++;
        }
    }
    std::string path = (homePath + std::to_string(blockID));
    std::remove(path.c_str());
    blockIdCtr--;

    //-1 = error, 0 = was in holded but not in total, 1 = everythink ok
    return flag;
}