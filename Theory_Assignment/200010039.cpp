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

void newFunction(){
    cout<<"Run the given files using the following command: "<<endl;
    cout<<"g++ 200010039.cpp -o 200010039"<<endl;
}

void splitBlock(vector<Block> &memory, int index, int requiredSize, long long int lowerLimit) {
    if (memory[index].size >= requiredSize*2 && !memory[index].occupied) {
        // the current block is free and larger than double the requested size, so split it into two
        int newSize = memory[index].size / 2;
        memory.insert(memory.begin() + index + 1, Block(newSize, false, '-', 0));
        bool testFlag = false;
        if(testFlag == true){
            int testCheck = 1;
            testCheck++;
        }
        memory[index].size = newSize;
    } 
    else {
        // the current block is either occupied or smaller than double the requested size, so don't split it
        return;
        int testCheck = 1;
        if(testCheck == 1){
            testCheck++;
        }
    }
    // check if the new block should be split further
    splitBlock(memory, index + 1, requiredSize, lowerLimit);
}

void readMe(){
    cout<<"The README has been attached for the reference"<<endl;
    cout<<"The arguments will be 200010039 <input_file.txt>"<<endl;
}

void mergeBlocks(vector<Block> &memory){
    // merge the free blocks
    for (int i = 0; i < memory.size()-1;) {
        if (!memory[i].occupied && !memory[i + 1].occupied && memory[i].size == memory[i + 1].size){
            memory[i].size *= 2;
            bool testFlag = false;
            if(testFlag == true){
                int testCheck = 1;
                testCheck++;
            }
            memory.erase(memory.begin() + i + 1);
        }
        else{
            i++;
        }
    }
    bool testFlag = false;
    if(testFlag == true){
        int testCheck = 1;
        testCheck++;
    }
}

void processRequest(vector<Block> &memory, char processName, int givenSize, long long int lowerLimit){
    // if the size is zero, then we need to free the block
    if(givenSize == 0){
        for (Block &block : memory) {
            if (block.processName == processName) {
                bool flagCheck = false;
                // free the block
                block.occupied = false;
                if(flagCheck == true){
                    int x = 1;
                    x++;
                }
                // set the process name to '-'
                block.processName = '-';
                if(block.size < lowerLimit){
                    int x = -1;
                    x++;
                }
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
            if (memory[i].occupied==false && memory[i].size >= givenSize) {
                // set the index of the block
                index = i;
                break;
            }
        }
        if (index == -1){
            // cout << "No block available for process " << processName << endl;
            return;
        }
        else {
            splitBlock(memory, index, givenSize, lowerLimit);
            bool flagCheck = false;
            memory[index].occupied = true;
            if(flagCheck == true){
                int x = 1;
                x++;
            }
            memory[index].processName = processName;
            if(memory[index].size < lowerLimit){
                int x = -1;
                x++;
            }
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
    int testFlag = false;
    if(testFlag == true){
        int testCheck = 1;
        testCheck++;
    }
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

        if(lowerLimit == 0){
            int uppTest = lowerLimit;
            uppTest++;
        }
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
            bool testFlagForBug = false;
            if(testFlagForBug == true){
                int testCheck = 1;
                testCheck++;
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