#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
using namespace std;
typedef pair<int, int> pi;

queue<pi> pq;
queue<pi> ioq;
vector<int> iocheck;
vector<queue<int>> process;
vector<int> ptot;
vector<int> tr_time;
vector<int> rtime_total;
vector<int> wait_time;
vector<int> btime_total;
vector<int> ptime_total;
vector<float> penalty;
int timeslice = 5;
int t_e = 0;
unordered_map<int, vector<int>> arrival;
bool processgoingon = false;
bool ioprocessgoingon = false;
int waitprocessgoingon = 0;
int iowaitprocessgoingon = 0;
int ongoingpid = -1;
int ioongoingpid = -1;

void readdata(string fl)
{
    string myText;
    queue<int> q;

    ifstream MyReadFile(fl);
    int j = 0;
    int sum = 0;
    while (getline(MyReadFile, myText))
    {
        if (myText[0] == '<')
        {
            continue;
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
                tr_time.push_back(0);
                wait_time.push_back(0);
                rtime_total.push_back(-1);
                btime_total.push_back(0);
                penalty.push_back(0);
                ptot.push_back(x);
                arrival[x].push_back(j);
                ptime_total.push_back(0);
                iocheck.push_back(0);
            }
            else
            {
                if (x != -1 && x != 0)
                {
                    if (i % 2 == 1)
                    {
                        sum += x;
                        btime_total[j] += x;
                    }
                    ptime_total[j] += x;
                    cout << x << ",";
                    q.push(x);
                }
            }
            i++;
        }
        j++;
        process.push_back(q);
    }

    cout << process.size() << " bsum= " << sum << endl;
    MyReadFile.close();
}

void startEvent(string fl)
{
    int time = 0;
    int tbreak = 0;
    for (;; time++, tbreak++)
    {

        if (arrival[time].size() > 0)
        {
            for (int prc = 0; prc < arrival[time].size(); prc++)
            {
                pq.push(make_pair(process[arrival[time][prc]].front(), arrival[time][prc]));
                cout << time << " "
                     << "Process arrived= " << arrival[time][prc] << " burst= " << process[arrival[time][prc]].front() << " " << endl;
                process[arrival[time][prc]].pop();
            }
        }
        if (!processgoingon && pq.size() > 0)
        {
            waitprocessgoingon = pq.front().first;
            ongoingpid = pq.front().second;
            if (rtime_total[ongoingpid] == -1)
                if (time - ptot[ongoingpid] - 1 < 0)
                    rtime_total[ongoingpid] = 0;
                else
                    rtime_total[ongoingpid] = time - ptot[ongoingpid] - 1;
            cout << time << ") "
                 << "Process Scheduled= " << pq.front().second << " " //<< " burst= " << pq.front().first << " "
                 << "Response Time= " << rtime_total[ongoingpid] << endl;
            ;
            pq.pop();
            processgoingon = true;
        }
        if (!ioprocessgoingon && ioq.size() > 0)
        {
            iowaitprocessgoingon = ioq.front().first;
            ioongoingpid = ioq.front().second;

            cout << time << ") "
                 << "ioProcess Scheduled= " << ioq.front().second << " timetake= " << ioq.front().first << " " << endl;
            ioq.pop();
            if (process[ioongoingpid].size() > 0)
            {
                iocheck[ioongoingpid] = 0;
                if (process[ioongoingpid].front() != 0)
                    pq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));

                pq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));
                process[ioongoingpid].pop();
            }
            ioprocessgoingon = true;
        }
        if (waitprocessgoingon >= 1)
        {
            waitprocessgoingon--;
        }
        if (iowaitprocessgoingon >= 1)
        {
            iowaitprocessgoingon--;
        }

        if (ioprocessgoingon && iowaitprocessgoingon == 0)
        {
            if (process[ioongoingpid].size() > 0)
            {
            }
            else
            {
                tr_time[ioongoingpid] = time - ptot[ioongoingpid] + 1;
                wait_time[ioongoingpid] = tr_time[ioongoingpid] - btime_total[ioongoingpid];
                penalty[ioongoingpid] = (btime_total[ioongoingpid] + wait_time[ioongoingpid]) * 1.0 / btime_total[ioongoingpid];
                cout << time << ") "
                     << "*" << ioongoingpid << " "
                     << "turnaround=" << tr_time[ioongoingpid] << " ";
                cout << "total Process time= " << ptime_total[ioongoingpid] << " ";
                cout << "waiting time=" << wait_time[ioongoingpid] << " " << endl;
                cout << "Burst Time " << btime_total[ioongoingpid] << " Penalty Ratio = " << penalty[ioongoingpid] << endl;
            }
            ioprocessgoingon = false;
        }

        if (processgoingon && waitprocessgoingon == 0)
        {

            if (process[ongoingpid].size() > 0)
            {
                if (iocheck[ongoingpid] == 0)
                {
                    iocheck[ongoingpid] = 1;
                    ioq.push(make_pair(process[ongoingpid].front(), ongoingpid));
                    process[ongoingpid].pop();
                }
            }
            else
            {
                tr_time[ongoingpid] = time - ptot[ongoingpid] + 1;
                wait_time[ongoingpid] = tr_time[ongoingpid] - ptime_total[ongoingpid];
                penalty[ongoingpid] = (btime_total[ongoingpid] + wait_time[ongoingpid]) * 1.0 / btime_total[ongoingpid];
                cout << time << ") "
                     << "*" << ioongoingpid << " "
                     << "turnaround=" << tr_time[ongoingpid] << " ";
                cout << "total Process time= " << ptime_total[ongoingpid] << " ";
                cout << "waiting time=" << wait_time[ongoingpid] << " " << endl;
                cout << "Burst Time " << btime_total[ongoingpid] << " Penalty Ratio = " << penalty[ongoingpid] << endl;
            }
            cout << "{" << ongoingpid << "}" << endl;
            tbreak = 0;
            processgoingon = false;
            if (pq.empty() && ioq.empty())
            {
                break;
            }
        }
        if (tbreak == timeslice)
        {
            if (processgoingon)
            {
                pq.push(make_pair(waitprocessgoingon, ongoingpid));
                processgoingon = false;
                tbreak = 0;
            }
        }
    }
    t_e = time;
    ofstream MyWriteFile(fl + "_" + to_string(timeslice) + "_rob.csv");
    float rt = 0, bst = 0, tat = 0, wt = 0, pta = 0, sz = rtime_total.size();
    cout << "_________________________Individual Process____________________________" << endl;
    MyWriteFile << "Process_ID,Response_Time,Wait_Time,Burst_Time,Turn_Around_Time,Penalty_Ratio\n";
    for (int i = 0; i < sz; i++)
    {
        MyWriteFile << i << "," << rtime_total[i] << "," << wait_time[i] << "," << btime_total[i] << "," << tr_time[i] << "," << penalty[i] << "\n";
        cout << "Process ID=" << i << " Response Time = " << rtime_total[i] << " Wait Time = " << wait_time[i] << " Burst Time = " << btime_total[i] << " Turn Around Time = " << tr_time[i] << " Penalty Ratio = " << penalty[i] << endl;
        rt += rtime_total[i];
        bst += btime_total[i];
        tat += tr_time[i];
        wt += wait_time[i];
        pta += penalty[i];
    }
    cout << "_________________________System Average____________________________"
         << "\n";
    MyWriteFile << sz * 1.0 / (t_e - 1) << " " << rt / sz << "," << wt / sz << "," << bst / sz << "," << tat / sz << "," << pta / sz << endl;
    cout << "Response Time = " << rt / sz << " Wait Time = " << wt / sz << " Burst Time = " << bst / sz << " Turn Around Time = " << tat / sz << " Penalty Ratio = " << pta / sz << endl;
    cout << "System throughput = " << sz * 1.0 / (t_e - 1) << endl;
    MyWriteFile.close();
}

int main(int argc, char *argv[])
{
    string fl(argc == 2 ? argv[1] : "process1.dat");
    timeslice = argc == 3 ? atoi(argv[2]) : 5;
    readdata(fl);
    startEvent(fl);
    return 0;
}
