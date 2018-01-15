#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <exception>

#include <sys/stat.h>

struct Block
    {
        int id;
        std::string data;
        bool consistent = true;
        time_t fetched = time(0);
        time_t lastused= time(0);

        bool operator<(const Block& right) const
        {
            return this->lastused < right.lastused;
        }
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

    std::string buffer;

private:

    bool blockExists(const int id);
    int newBlockID();
    bool saveLRU();
    int blockIdCtr = 0;
    int bufferSize;
    std::string homePath;
    std::ifstream readS;
    std::ofstream writeS;
    std::vector<int> totalBlocks;
    std::map<int, Block> holdedBlocks; //int: lastTimeUsed
};

#endif /* STORAGEMANAGER_H */

