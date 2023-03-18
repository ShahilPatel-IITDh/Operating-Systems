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

		// Check if the set can hold more pages
		if (mainMemorySet.size() < mainMemorySize){
			if(swapSet.find(pages[i]) != swapSet.end()){
                // remove the page from swap set
				// cout<<"Page "<<pages[i]<<" is in swap set"<<endl;
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
				// val is the least recently used page
				int val = mainMemoryMap[pages[i]];
				// Remove the mainMemory page from the set
				mainMemorySet.erase(val);
				// insert the current page in the set
				swapSet.erase(pages[i]);
				// Increment page faults
				pageFaults++;
				// push the current page into the queue
				mainMemorySet.insert(pages[i]);
				swapSet.insert(val);
            }

			// Check if current page is not already present in the set
			else if (mainMemorySet.find(pages[i]) == mainMemorySet.end()){
				// Find the least recently used pages
				// that is present in the set
				int lru = INT_MAX, val;
				for (auto it=mainMemorySet.begin(); it!=mainMemorySet.end(); it++){
					if (mainMemoryMap[*it] < lru){
						lru = mainMemoryMap[*it];
						val = *it;
					}
				}

				// Remove the mainMemoryMap page
				mainMemorySet.erase(val);

				// insert the current page
				mainMemorySet.insert(pages[i]);

				// Increment page faults
				pageFaults++;
			}

			// Update the current page index
			mainMemoryMap[pages[i]] = i;
		}
	}

	return pageFaults;
}

// Driver code
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

    // open input file for reading
    ifstream file(argv[4]);

    if (!file.is_open()) {
        cout << "Error opening input file!" << endl;
        return 1;
    }

    // simulate FIFO page replacement policy
    // file>>pageNum means read a number from the file and store it in pageNum
    vector<int> pages(numPages);
    int i=0;
    int pageNum=0;
    while (file >> pageNum) {
        pages[i] = pageNum;
        i++;
    }

    // close input file
    file.close();
    // print number of page faults
    cout << "Number of page faults: " << pageFaults(pages, numPages, numFrames, numBlocks) << endl;
    return 0;
}
