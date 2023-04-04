#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class BuddyManager {
public:
    BuddyManager(int u, int l) : U(u), L(l), n((1 << u) - 1), free(n + 1, true), size(n + 1) {
        for (int i = 0; i <= n; ++i) {
            size[i] = 1 << (U - i + L - 1);
        }
    }

    void allocate(char pid, int s) {
        int k = ceil_log2(s + L);
        int i = first_fit(k);
        if (i == -1) {
            throw runtime_error("memory allocation failed");
        }
        split(i, k);
        allocated[pid] = i;
        free[i] = false;
    }

    void deallocate(char pid) {
        int i = allocated[pid];
        free[i] = true;
        coalesce(i);
        allocated.erase(pid);
    }

    void print_state() const {
        cout << "Processes:" << endl;
        for (const auto& entry : allocated) {
            char pid = entry.first;
            int i = entry.second;
            int s = size[i] - L;
            cout << pid << " : " << s << endl;
        }
        cout << "Free Blocks:" << endl;
        vector<pair<int, int>> blocks;
        for (int i = 0; i <= n; ++i) {
            if (free[i]) {
                int s = size[i] - L;
                blocks.push_back({i, s});
            }
        }
        sort(blocks.begin(), blocks.end());
        for (const auto& block : blocks) {
            int i = block.first;
            int s = block.second;
            cout << "Free Block : " << s << endl;
        }
    }

private:
    int U, L, n;
    vector<bool> free;
    vector<int> size;
    unordered_map<char, int> allocated;

    int ceil_log2(int x) const {
        int k = 0;
        while ((1 << k) < x) {
            ++k;
        }
        return k;
    }

    int first_fit(int k) const {
        int i = 0;
        while (i <= n) {
            if (free[i] && size[i] >= (1 << (U - k + L))) {
                return i;
            }
            ++i;
        }
        return -1;
    }

    void split(int i, int k) {
        while (size[i] > (1 << (U - k + L))) {
            int j = i + (1 << (k - 1));
            free[j] = true;
            size[j] = size[i] / 2;
            size[i] = size[j];
            k--;
        }
    }

    void coalesce(int i) {
        int k = ceil_log2(size[i] + L);
        while (true) {
            int j = i ^ (1 << (U - k + L));
            if (j > n || !free[j]) {
                break;
            }
            free[i] = true;
            free[j] = true;
            size[i] += size[j];
            i = min(i, j);
            k++;
        }
    }
};

int main() {
    int num_cases;
    cin >> num_cases;
    
    // Process each test case
    for (int i = 0; i < num_cases; i++) {
        int upper_size, lower_size;
        cin >> upper_size >> lower_size;

        // Initialize buddy memory manager
        BuddyManager buddy(upper_size, lower_size);

        string line;
        getline(cin, line); // consume blank line

        while (getline(cin, line) && !line.empty()) {
            char process_id;
            int request_size;
            stringstream ss(line);
            ss >> process_id >> request_size;

            if (request_size == 0) {
                buddy.release(process_id);
            } else {
                buddy.allocate(process_id, request_size);
            }
        }

        // Print current state of buddy memory
        cout << "Case " << i+1 << ":\n";
        buddy.print_state();
        if (i != num_cases - 1) {
            cout << endl; // print blank line between cases
        }
    }

    return 0;
}