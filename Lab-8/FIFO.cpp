#include<bits/stdc++.h>
using namespace std;

// Function to find page faults using FIFO
int pageFaults(vector<int>& pages, int numPages, int mainMemorySize, int blocks){
	// To represent set of current pages. We use an unordered_set so that we quickly check if a page is present in set or not
	unordered_set<int> mainMemorySet;
    // To represent set of pages in swap
    unordered_set<int> swapSet;

	// To store the pages in FIFO manner
	queue<int> mainMemory;

	// Start from initial page
	int pageFaults = 0;
	for (int i=0; i<numPages; i++){
		// Check if the set can hold more pages
        if(swapSet.size()==blocks){
            cout<<"Swap set is full"<<endl;
            exit(0);
        }

        // If the set is not full then insert it into set if not present already which represents page fault
		if (mainMemorySet.size() < mainMemorySize){
            // Check if current page is already present in the swap set, if yes then remove it from swap set and insert it into main memory set and queue
            if(swapSet.find(pages[i]) != swapSet.end()){
                // cout<<"Page "<<pages[i]<<" is in swap set"<<endl;
                // remove the page from swap set
                swapSet.erase(pages[i]);
                // insert the page into main memory set
                mainMemorySet.insert(pages[i]);
                // push the page into main memory queue
                mainMemory.push(pages[i]);
                // increment page faults
                pageFaults++;
            }

			else if(mainMemorySet.find(pages[i]) == mainMemorySet.end()){
				// Insert the current page into the set
				mainMemorySet.insert(pages[i]);
				// increment page fault
				pageFaults++;
				// Push the current page into the queue
				mainMemory.push(pages[i]);
			}
		}

		// If the set is full then need to perform FIFO i.e. remove the first page of the queue from set and queue both and insert the current page
		else{
            if(swapSet.find(pages[i]) != swapSet.end()){
                // cout<<"Control is here"<<endl;
                // remove the page from swap set
                int val = mainMemory.front();
                // Pop the first page from the queue
                mainMemory.pop();
                // Remove the mainMemory page from the set
                mainMemorySet.erase(val);
                // insert the current page in the set
                swapSet.erase(pages[i]);
                // push the current page into the queue
                mainMemorySet.insert(pages[i]);
                // Increment page faults
                mainMemory.push(pages[i]);
                // add the page to swap set
                pageFaults++;
                // add the page to swap set
                swapSet.insert(val);
            }

			else if (mainMemorySet.find(pages[i]) == mainMemorySet.end()){
				// Store the first page in the queue to be used to find and erase the page from the set
				int val = mainMemory.front();
				// Pop the first page from the queue
				mainMemory.pop();
				// Remove the mainMemory page from the set
				mainMemorySet.erase(val);
				// insert the current page in the set
				mainMemorySet.insert(pages[i]);
				// push the current page into the queue
				mainMemory.push(pages[i]);
				// Increment page faults
				pageFaults++;
                // add the page to swap set
                swapSet.insert(val);
			}
		}
	}
	return pageFaults;
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

    // open input file for reading
    ifstream file(argv[4]);
    // create a csv file 
    ofstream csvFIFO;
    csvFIFO.open("csvFIFO.csv");

    // check if file opened successfully
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
    // Print the number of page faults along with the numFrames in csv file
    int frames=5;
	while(frames<=numFrames){
        csvFIFO << frames << "," << pageFaults(pages, numPages, frames, numBlocks) << endl;
        frames+=5;
    }
    // close csv file
    csvFIFO.close();
    // print number of page faults
    cout << "Number of page faults: " << pageFaults(pages, numPages, numFrames, numBlocks) << endl;
    return 0;
}
