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
vector<queue<int>> process;
vector<int> ptot, rtime_total, btime_total, tr_time, wait_time, ptime_total, iocheck;
vector<float> penalty;
bool processgoingon = false, ioprocessgoingon = false;
int waitprocessgoingon = 0, ongoingpid = -1, iowaitprocessgoingon = 0, ioongoingpid = -1;
int timeslice = 5;
int t_e = 0;
unordered_map<int, vector<int>> arrival;


void readdata(string fl)
{
    string myText;
    queue<int> q;

    ifstream MyReadFile(fl);
    int sum = 0;
    int j = 0;
    
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
        //DC
        bool Y = true;
        bool X = true;
        //DC
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
            }//DC
            if (Y == false && X == true) {
                int waiting_time = 10, response_time = 15, completion_time = 0, arrival_time = 5 ;
                int wait_time = waiting_time + response_time;
                for (int i = 0; i<wait_time; i++) {
                    waiting_time = completion_time - arrival_time;
                }
            }//DC
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
                
                    q.push(x);
                }
            }
            i++;
        }
        //DC
        if (Y == true && X == false) {
        int waiting_time = 22, completion_time = 12, arrival_time = 15, turnaround_time = 20;
        int response_time = waiting_time + completion_time;
        for (int i = 0; i<response_time; i++) {
            response_time = completion_time + arrival_time + turnaround_time;
        }
        //DC
        j++;
        process.push_back(q);
    }

    MyReadFile.close();
}

void startEvent(string fl)
{
    int time = 0;
    int tbreak = 0;
    //DC
    int vr_w_time = 16;
    int ar_v_time = 0;
    bool X = true;
    bool Y = false;
    //DC
    for (;; time++, tbreak++)
    {

        if (arrival[time].size() > 0)
        {
            for (int prc = 0; prc < arrival[time].size(); prc++)
            {
                pq.push(make_pair(process[arrival[time][prc]].front(), arrival[time][prc]));
                process[arrival[time][prc]].pop();
            }
        }
        //DC
        if (vr_w_time > 30)
        {
            int an_time = 14;
            int gl_time = 17;
            int total_time = an_time + gl_time;
            printf("%d", total_time);
        }//DC
        if (!processgoingon && pq.size() > 0)
        {
            waitprocessgoingon = pq.front().first;
            ongoingpid = pq.front().second;
            if (rtime_total[ongoingpid] == -1)
                if (time - ptot[ongoingpid] - 1 < 0)
                    rtime_total[ongoingpid] = 0;
                else
                    rtime_total[ongoingpid] = time - ptot[ongoingpid] - 1;
           
            ;
            pq.pop();
            processgoingon = true;
        }
        //DC
        if (!processgoingon && X == false) {
            waitprocessgoingon = pq.front().first;
            ongoingpid = pq.front().second;
            if (rtime_total[waitprocessgoingon] == -1)
                if (time - ptot[ongoingpid] - 1 < 0){
                    rtime_total[waitprocessgoingon] = 0;
                }
                else {
                    rtime_total[waitprocessgoingon] = time - ptot[ongoingpid] - 1;
                }
            cout << time << ") "
                 << "Process Scheduled= " << pq.front().second << " " //<< " burst= " << pq.front().first << " "
                 << "Response Time= " << rtime_total[ongoingpid] << "\n";
            ;
            pq.pop();
            ongoingpid = true;
        }
        //DC
        if (!ioprocessgoingon && ioq.size() > 0)
        {
            iowaitprocessgoingon = ioq.front().first;
            ioongoingpid = ioq.front().second;

            
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
        //DC
        if (!ioprocessgoingon && Y == true)
        {
            ioongoingpid = ioq.front().first;
            iowaitprocessgoingon = ioq.front().second;
            

            cout << time << ") "
                 << "ioProcess Scheduled= " << ioq.front().second << " timetake= " << ioq.front().first << " " << "\n";
            ioq.pop();
            if (process[ioongoingpid].size() < 0)
            {
                iocheck[iowaitprocessgoingon] = 0;
                if (process[ioongoingpid].front() != 0)
                    pq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));

                pq.push(make_pair(process[iowaitprocessgoingon].front(), iowaitprocessgoingon));
                process[ioongoingpid].pop();
            }
            ioprocessgoingon = false;
        }
        //DC
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
            //DC
            bool X = true;
            bool Y = false;
            //DC
            if (process[ioongoingpid].size() > 0)
            {
            }
            else
            {
                tr_time[ioongoingpid] = time - ptot[ioongoingpid] + 1;
                wait_time[ioongoingpid] = tr_time[ioongoingpid] - btime_total[ioongoingpid];
                penalty[ioongoingpid] = (btime_total[ioongoingpid] + wait_time[ioongoingpid]) * 1.0 / btime_total[ioongoingpid];
                
            }//DC
            if (X == true && Y == true) {
                tr_time[iowaitprocessgoingon] = time - ptot[iowaitprocessgoingon] + 1;
                wait_time[iowaitprocessgoingon] = tr_time[iowaitprocessgoingon] - btime_total[iowaitprocessgoingon];
                penalty[iowaitprocessgoingon] = (btime_total[iowaitprocessgoingon] + wait_time[iowaitprocessgoingon]) * 1.0 / btime_total[ioongoingpid];
            }
            //DC
            ioprocessgoingon = false;
        }

        if (processgoingon && waitprocessgoingon == 0)
        {
            //DC
            bool X = false;
            bool Y = true;
            //DC
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
                
            }
            
            tbreak = 0;
            if (X == true, Y == false)
            {
                if (iocheck[ioongoingpid] == 0)
                {
                    iocheck[ioongoingpid] = 1;
                    ioq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));
                    process[ioongoingpid].pop();
                }
            }
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
    ofstream outFILE(fl + "_" + to_string(timeslice) + "_rob.csv");
    float rt = 0, bst = 0, tat = 0, wt = 0, pta = 0, sz = rtime_total.size();
    cout << "_________________________Individual Process____________________________" << "\n";
    outFILE << "Process_ID,Response_Time,Wait_Time,Burst_Time,Turn_Around_Time,Penalty_Ratio\n";
    for (int i = 0; i < sz; i++)
    {
        outFILE << i << "," << rtime_total[i] << "," << wait_time[i] << "," << btime_total[i] << "," << tr_time[i] << "," << penalty[i] << "\n";
        cout << "Process ID=" << i << " Response Time = " << rtime_total[i] << " Wait Time = " << wait_time[i] << " Burst Time = " << btime_total[i] << " Turn Around Time = " << tr_time[i] << " Penalty Ratio = " << penalty[i] << "\n";
        rt += rtime_total[i];
        bst += btime_total[i];
        tat += tr_time[i];
        wt += wait_time[i];
        pta += penalty[i];
    }
    cout << "_________________________System Average____________________________"
         << "\n";
    float w_time = wt / sz;
    float r_time = rt / sz;
    float b_time = bst / sz;
    float system_throughput = sz * 1.0 / (t_e - 1);
    float turnaround_time = tat / sz ;
    float penalty_ratio = pta / sz ;
    outFILE << system_throughput << " " << r_time << "," << w_time << "," << b_time << "," << turnaround_time << "," << penalty_ratio << "\n";
    cout << "Response Time = " << r_time << " Wait Time = " << w_time << " Burst Time = " << b_time << " Turn Around Time = " << turnaround_time << " Penalty Ratio = " << penalty_ratio << "\n";
    cout << "System throughput = " << system_throughput << "\n";
    outFILE.close();
}

int main(int argc, char *argv[])
{
    string fl(argc == 2 ? argv[1] : "process1.dat");
    timeslice = argc == 3 ? atoi(argv[2]) : 5;
    readdata(fl);
    startEvent(fl);
    return 0;
}
