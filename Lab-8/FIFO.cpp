#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]) {
    int numPages, numFrames, numBlocks, pageNum, pageFaults = 0;

    queue<int> FIFOQueue;
    unordered_map<int, bool> pageTable;
    unordered_map<int, bool> swapTable;

    // check if correct number of arguments are passed
    if (argc != 5) {
        cout << "Invalid number of arguments!" << endl;
        return 1;
    }

    // parse input arguments
    numPages = atoi(argv[1]);
    numFrames = atoi(argv[2]);
    numBlocks = atoi(argv[3]);

    // open input file for reading
    ifstream file(argv[4]);

    if (!file.is_open()) {
        cout << "Error opening input file!" << endl;
        return 1;
    }

    // simulate FIFO page replacement policy
    // file>>pageNum means read a number from the file and store it in pageNum
    while (file >> pageNum) {
        // check if page is already in main memory
        if (pageTable.find(pageNum) != pageTable.end()) {
            continue;
        }
        
        // if page is not in main memory and swapTable, add it to the queue and page table
        else if(pageTable.find(pageNum) == pageTable.end()){
            if(swapTable[pageNum] == false){
                FIFOQueue.push(pageNum);
                pageTable[pageNum] = true;
                pageFaults++;
            }
            else if(swapTable[pageNum] == true){
                if(FIFOQueue.size() > numFrames){
                    pageTable[FIFOQueue.front()] = false;
                    swapTable[FIFOQueue.front()] = true;
                    FIFOQueue.pop();
                }
                swapTable[pageNum] = false;
                pageTable[pageNum] = true;
                FIFOQueue.push(pageNum);
            }
        }

        // if the queue size exceeds the number of frames, remove the oldest page from the queue and page table
        if (FIFOQueue.size() > numFrames) {
            pageTable[FIFOQueue.front()] = false;
            swapTable[FIFOQueue.front()] = true;
            FIFOQueue.pop();
        }
    }

    // close input file
    file.close();
    // print page fault rate
    cout << "Page Faults: " << pageFaults << endl;
    return 0;
}
