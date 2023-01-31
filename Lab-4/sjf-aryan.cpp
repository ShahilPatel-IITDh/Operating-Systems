#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;
typedef pair<int, int> pi;

priority_queue<pi, vector<pi>, greater<pi>> pq;
vector<queue<int>> process;
unordered_map<int, vector<int>> arrival;
bool processgoingon = false;
int waitprocessgoingon = 0;
int ongoingpid = -1;
void readdata(string fl)
{
    // ofstream MyFile(fl);
    string myText;
    queue<int> q;

    ifstream MyReadFile(fl);
    if (getline(MyReadFile, myText))
    {
        if (getline(MyReadFile, myText))
        {
            if (getline(MyReadFile, myText))
            {
            }
        }
    }
    int j = 0;
    int sum = 0;
    while (getline(MyReadFile, myText))
    {
        if (myText == "</pre></body></html>")
        {
            break;
        }
        stringstream ss(myText);
        string num;
        queue<int> q;
        int i = 0;
        while (getline(ss, num, ' '))
        {

            stringstream ssnm(num);
            int x = 0;
            ssnm >> x;
            if (i == 0)
            {
                arrival[x].push_back(j);
                i++;
            }
            else
            {
                if (x != -1)
                {
                    sum += x;
                    cout << x << ",";
                    q.push(x);
                }
            }
        }
        cout << endl;
        j++;
        process.push_back(q);
    }

    // Write to the file
    cout << process.size() << "Files can be tricky, but it is fun enough! " << sum << endl;

    // Close the file
    MyReadFile.close();
}

void startEvent()
{

    for (int time = 0;; time++)
    {
        cout << time << " ";
        if (arrival[time].size() > 0)
        {
            for (int prc = 0; prc < arrival[time].size(); prc++)
            {
                pq.push(make_pair(process[arrival[time][prc]].front(), arrival[time][prc]));
                cout << "Process arrived= " << arrival[time][prc] << " burst= " << process[arrival[time][prc]].front() << " ";
                process[arrival[time][prc]].pop();
            }
        }
        if (!processgoingon)
        {
            waitprocessgoingon = pq.top().first;
            ongoingpid=pq.top().second;
            cout << "Process Scheduled= " << pq.top().second << " burst= " << pq.top().first<<" " ;
            pq.pop();
            processgoingon = true;
        }
        if (waitprocessgoingon >= 1)
        {
            waitprocessgoingon--;
        }
        if (waitprocessgoingon == 0)
        {
            if (process[ongoingpid].size() > 0)
            {
                pq.push(make_pair(process[ongoingpid].front(),ongoingpid));
                process[ongoingpid].pop();
            }
            if (pq.empty())
            {
                break;
            }
            processgoingon = false;
        }
        cout<<endl;
    }
}

int main(int argc, char *argv[])
{
    string fl = "process1.dat";
    readdata(fl);
    startEvent();
    return 0;
}
