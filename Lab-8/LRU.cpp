#include<bits/stdc++.h>
using namespace std;

// Function to find page faults using mainMemoryMap
int pageFaults(vector<int>& pages, int numPages, int mainMemorySize, int blocks){
	// To represent set of current pages. We use an unordered_set so that we quickly check if a page is present in main memory or not
	unordered_set<int> mainMemorySet;

	// To store least recently used mainMemoryMap of pages.
	unordered_map<int, int> mainMemoryMap;

	// To store the pages in swap blocks
	unordered_set<int> swapSet;

	// Start from initial page
	int pageFaults = 0;
	for (int i=0; i<numPages; i++){
		// Check if the set can hold more pages
        if(swapSet.size()==blocks){
            cout<<"Swap set is full"<<endl;
            exit(0);
        }

		if(swapSet.size()==0){
			bool checkSize = false;
			for(auto i=mainMemorySet.begin(); i!=mainMemorySet.end(); i++){
				if(mainMemoryMap[*i] == INT_MAX){
					checkSize = true;
					break;
				}
			}
		}

		// Check if the set can hold more pages
		if (mainMemorySet.size() < mainMemorySize){
			if(swapSet.find(pages[i]) != swapSet.end()){
                // remove the page from swap set
                swapSet.erase(pages[i]);
                // insert the page into main memory set
                mainMemorySet.insert(pages[i]);
                // push the page into main memory queue
                pageFaults++;
            }

			// Insert it into set if not present already which represents page fault
			else if (mainMemorySet.find(pages[i]) == mainMemorySet.end()){
				mainMemorySet.insert(pages[i]);
				// increment page fault
				pageFaults++;
			}
			// Store the recently used index of each page
			mainMemoryMap[pages[i]] = i;
		}

		// If the set is full then need to perform lru i.e. remove the least recently used page and insert the current page
		else{
			if(swapSet.find(pages[i]) != swapSet.end()){
				// Increment page faults
				pageFaults++;

				// val is the least recently used page
				int val = mainMemoryMap[pages[i]];

				// Remove the mainMemory page from the set
				mainMemorySet.erase(val);

				// insert the current page in the set
				swapSet.erase(pages[i]);
				
				// push the current page into the queue
				mainMemorySet.insert(pages[i]);
				// push the least recently used page into the swap set
				swapSet.insert(val);
            }

			// Check if current page is not already present in the set
			else if (mainMemorySet.find(pages[i]) == mainMemorySet.end()){
				// Find the least recently used pages that is present in the set
				int lru = INT_MAX;
				int val;
				for (auto i=mainMemorySet.begin(); i!=mainMemorySet.end(); i++){
					// Check if the current page is less than least recently used pages, if yes then update lru, val is the least recently used page
					if (mainMemoryMap[*i] < lru){
						// Update the least recently used pages, val and lru where lru is the least recently used page and val is the page number
						lru = mainMemoryMap[*i];
						val = *i;
					}
				}
				// Increment page faults
				pageFaults++;

				// Remove the mainMemoryMap page
				mainMemorySet.erase(val);

				// insert the current page
				mainMemorySet.insert(pages[i]);
			}

			// Update the current page index
			mainMemoryMap[pages[i]] = i;
		}
	}
	return pageFaults;
}

// Function to take input from the input file
void takeInput(ifstream& inputFile, vector<int>& pages, int numPages, int pageNum, int i){
	while (inputFile >> pageNum) {
		pages[i] = pageNum;
		i++;
		if(i==numPages){
			break;
		}
	}
}


void generateCSV(ofstream& csvLRU, int frames, int numFrames, vector<int>& pages, int numPages, int numBlocks){
	while(frames<=numFrames){
		csvLRU << frames << "," << pageFaults(pages, numPages, frames, numBlocks) << endl;
		frames++;
	}
}

int main(int argc, char *argv[]){

    int numPages, numFrames, numBlocks;
    // check if correct number of arguments are passed
    if (argc != 5) {
        cout << "Invalid number of arguments!" << endl;
        return 1;
    }

    // parse input arguments
    numPages = atoi(argv[1]);
    numFrames = atoi(argv[2]);
    numBlocks = atoi(argv[3]);

    // open input File for reading
    ifstream inputFile(argv[4]);
	// create a csv File
	ofstream csvLRU;
	csvLRU.open("csvLRU.csv");

    if (!inputFile.is_open()) {
        cout << "Error opening input inputFile!" << endl;
        return 1;
    }

    // simulate FIFO page replacement policy
    // inputFile>>pageNum means read a number from the input file and store it in pageNum
    vector<int> pages(numPages);
    int i=0;
    int pageNum=0;
	takeInput(inputFile, pages, numPages, pageNum, i);

    // close input File
    inputFile.close();

	// print the number of page faults along with frames used in csv file
	generateCSV(csvLRU, 1, numFrames, pages, numPages, numBlocks);

	// close csv file
	csvLRU.close();
    return 0;
}
