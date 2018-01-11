#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>


struct Block{
    
    int id; 
    std::string data;
    time_t fetched = time(0);
    time_t lastused = time(0);
};

class StorageManager 
{
public:
    
    StorageManager();
    StorageManager(std::string homepath);
    StorageManager(std::string homepath, int buffersize);
    StorageManager(const StorageManager& orig);
    virtual ~StorageManager();
    
    void init_store(int total_blocks, int ram_blocks);
    void cleanup_store();
    int readCacheBlock(int blockID, std::string &buffer);
    int writeCacheBlock(int BlockID, std::string buffer);
    int freeCacheBlock(int blockID);
    
private:

    int newId();
    int blockIdCtr = 0;
    int bufferSize; 
    std::string homePath;
    std::string buffer; 
    std::vector<int> totalBlocks;
    std::map<Block, bool> holdedBlocks;     //bool: consistent y/n
    
    std::ifstream readS;
    std::ofstream writeS;
    
};

#endif /* STORAGEMANAGER_H */

