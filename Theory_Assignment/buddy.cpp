#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct Block{
    bool occupied;
    int size;
    char process;
};

Block generateBlock(int size, bool occupied, char process) {
    Block block;
    block.size = size;
    block.occupied = occupied;
    block.process = process;
    return block;
}

void splitBlock(vector<Block> &memory, int index, int target_size) {
    while (memory[index].size > target_size) {
        memory.insert(memory.begin() + index + 1, generateBlock(memory[index].size / 2, false, '-'));
        memory[index].size /= 2;
    }
}

void merge_blocks(vector<Block> &memory) {
    for (int i = 0; i < memory.size() - 1;) {
        if (!memory[i].occupied && !memory[i + 1].occupied && memory[i].size == memory[i + 1].size) {
            memory[i].size *= 2;
            memory.erase(memory.begin() + i + 1);
        } else {
            i++;
        }
    }
}

void processRequest(vector<Block> &memory, char process, int size) {
    if (size == 0) {
        for (Block &block : memory) {
            if (block.process == process) {
                block.occupied = false;
                block.process = '-';
                break;
            }
        }
        merge_blocks(memory);
    } else {
        int index = -1;
        for (int i = memory.size() - 1; i >= 0; i--) {
            if (!memory[i].occupied && memory[i].size >= size) {
                index = i;
            }
        }
        if (index != -1) {
            splitBlock(memory, index, size);
            memory[index].occupied = true;
            memory[index].process = process;
        }
    }
}

int main() {
    int testCases;
    cin >> testCases;
    while (testCases--){

        int U;
        int L;
        cin >> U >> L;

        vector<Block> memory = {generateBlock(pow(2, U), false, '-')};

        string requestLine;
        getline(cin, requestLine);

        while (getline(cin, requestLine) && !requestLine.empty()) {
            char process = requestLine[0];
            int size = stoi(requestLine.substr(2));
            processRequest(memory, process, size);
        }

        for (const Block &block : memory) {
            if (block.occupied) {
                cout << block.process << ": " << block.size << endl;
            } else {
                cout << "Free Block: " << block.size << endl;
            }
        }
        if (testCases) {
            cout << endl;
        }
    }
    return 0;
}