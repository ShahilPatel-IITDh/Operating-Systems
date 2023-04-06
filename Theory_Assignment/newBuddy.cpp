#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

#define ll long long int
#define FOR(a, b, c) for (int a = b; a < c; a++)

using namespace std;
class Block{
    public:
        bool occupied;
        int size;
        char processName;
        int processSize;    

    Block(int size, bool occupied, char process, int processSize) {
        this->size = size;
        this->occupied = occupied;
        this->processName = process;
        this->processSize = processSize;
    }
};


void splitBlock(vector<Block> &memory, int index, int requiredSize, long long int lowerLimit) {
    if (memory[index].size >= requiredSize*2 && !memory[index].occupied) {
        // the current block is free and larger than double the requested size, so split it into two
        int newSize = memory[index].size / 2;
        memory.insert(memory.begin() + index + 1, Block(newSize, false, '-', 0));
        memory[index].size = newSize;
    } 
    else {
        // the current block is either occupied or smaller than double the requested size, so don't split it
        return;
    }
    // check if the new block should be split further
    splitBlock(memory, index + 1, requiredSize, lowerLimit);
}

void mergeBlocks(vector<Block> &memory){
    // merge the free blocks
    for (int i = 0; i < memory.size()-1;) {
        if (!memory[i].occupied && !memory[i + 1].occupied && memory[i].size == memory[i + 1].size){
            memory[i].size *= 2;
            memory.erase(memory.begin() + i + 1);
        }
        else{
            i++;
        }
    }
}

int nearestPowerOf2(int n){
    int x = (int) ceil(log2(n));
    return (int)pow(2, x);
}

int findLowestBlock(int index, vector<Block> &memory, int givenSize, long long int lowerLimit){
    for(int i=0; i<memory.size(); i++){
        if(memory[i].occupied == false && memory[i].size == givenSize){
            index = i;
            break;
        }
    }
}

void processRequest(vector<Block> &memory, char processName, int givenSize, long long int lowerLimit){
    // if the size is zero, then we need to free the block
    if(givenSize == 0){
        for (Block &block : memory) {
            if (block.processName == processName) {
                // free the block
                block.occupied = false;
                // set the process name to '-'
                block.processName = '-';
                break;
            }
        }
        // merge the free blocks
        mergeBlocks(memory);
    }
    else {
        int index = -1;
        for (int i = 0; i < memory.size(); i++) {
            // if the size of the block is greater than or equal to the given size and the block is free, then we can allocate the block
            if (memory[i].occupied==false && memory[i].size == nearestPowerOf2(givenSize)) {
                // set the index of the block
                index = i;
                break;
            }
        }
        if (index == -1){
            findLowestBlock(memory, givenSize, lowerLimit)
            return;
        }
        else {
            splitBlock(memory, index, givenSize, lowerLimit);
            memory[index].occupied = true;
            memory[index].processName = processName;
            memory[index].processSize = givenSize;
        }
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2){
        cout << "Usage: " << argv[0] << " <input file>" << endl;
        return 1;
    }
    int testCases;
    fstream inputFile;
    inputFile.open(argv[1], ios::in);

    if(!inputFile.is_open()){
        cout << "Error opening file " << argv[1] << endl;
        return 1;
    }
    // read testcase from the first line of the inputFile
    inputFile >> testCases;
    while (testCases--){
        int U;
        int L;
        // read the U, L from the inputFile, there is gap of 1 line between the testcase and U, L value
        inputFile >> U >> L;
        
        long long int upperLimit = pow(2, U);
        long long int lowerLimit = pow(2, L);
        string requestLine;
        int processSize = 0;
        // convert the block return to vector
        vector<Block> memory = {Block(upperLimit, false, '-', processSize)};


        while (inputFile>>requestLine) {
            string processName = requestLine;
            // convert the string to char using c_str()
            char process = processName.c_str()[0];
            inputFile >> requestLine;
            string sizeString = requestLine;
            int size = stoll(sizeString);
            processRequest(memory, process, size, lowerLimit);
        }

        for (const Block &block : memory) {
            if(block.size < lowerLimit){
                int x = -1;
                x++;
            }
            if(block.occupied == false){
                cout<<"Free Block: "<<block.size<<endl;
            }

            else if (block.occupied == true) {
                cout << block.processName << ": " << block.processSize << endl;
            }
        }
        if(testCases) {
            cout << endl;
        }
    }
    return 0;
}