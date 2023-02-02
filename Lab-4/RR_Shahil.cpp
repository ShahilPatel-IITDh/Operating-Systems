#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>

using namespace std;

// Global Variables declarations

vector<queue<int>> Process;
queue<pair<int, int>> IO_Queue, CPU_Queue;
vector<int> iocheck, ptot, turnaroundTime, rtime_total, wait_time, burstTime, ptime_total;
vector<float> penalty;

int waitprocessgoingon = 0, IO_Wait = 0, ongoingpid = -1, ioongoingpid = -1;

// If the timeslice is not specified through the input file, then 3 will be taken as default value .

int timeslice = 3;
int time1 = 0;

unordered_map<int, vector<int>> processArrival;
bool process_On = false, IO_Process_On = false;
    
void readFile(string fileLine){
    string inputText;
    ifstream InputFile(fileLine);
    int j=0, sum=0;

    while (getline(InputFile, inputText)){
        // Ignore the lines having <
        if (inputText[0] == '<'){
            continue;
        }

        string num;
        queue<int> q;

        stringstream ss(inputText);

        int i = 0;
        while (getline(ss, num, ' ')){
            stringstream ssnm(num);
            int x = 0;
            ssnm >> x;

            if (i == 0){
                turnaroundTime.push_back(0);
                wait_time.push_back(0);
                rtime_total.push_back(-1);
                burstTime.push_back(0);
                penalty.push_back(0);
                ptot.push_back(x);

                processArrival[x].push_back(j);
                ptime_total.push_back(0);
                iocheck.push_back(0);
            }

            else{
                if (x != -1 && x != 0){
                    if (i % 2 == 1){
                        sum += x;
                        burstTime[j] += x;
                    }
                    ptime_total[j] += x;
                    q.push(x);
                }
            }
            i++;

        }
        j++;
        Process.push_back(q);
    }
    cout << "Number of processes: " << Process.size() << " Size of Process = " << sum << endl;
    InputFile.close();

// void Execution(string fl){
    int time = 0;
    int tbreak = 0;
    int x = 0;
    bool flags = false;
    bool checkFull = false;
    
    for (;; time++, tbreak++){

        if (processArrival[time].size() > 0){
            for (int prc = 0; prc < processArrival[time].size(); prc++){
                CPU_Queue.push(make_pair(Process[processArrival[time][prc]].front(), processArrival[time][prc]));
                Process[processArrival[time][prc]].pop();
            }
        }
        if (!process_On && CPU_Queue.size() > 0){
            waitprocessgoingon = CPU_Queue.front().first;

            ongoingpid = CPU_Queue.front().second;
            if (rtime_total[ongoingpid] == -1)
                if (time - ptot[ongoingpid] - 1 < 0){
                    rtime_total[ongoingpid] = 0;
                }
                else{
                    rtime_total[ongoingpid] = time - ptot[ongoingpid] - 1;
                }
            CPU_Queue.pop();
            process_On = true;
        }

        if (!IO_Process_On && IO_Queue.size() > 0){
            IO_Wait = IO_Queue.front().first;
            ioongoingpid = IO_Queue.front().second;
            IO_Queue.pop();

            if (Process[ioongoingpid].size() > 0){

                iocheck[ioongoingpid] = 0;
                if (Process[ioongoingpid].front() != 0){
                    CPU_Queue.push(make_pair(Process[ioongoingpid].front(), ioongoingpid));
                }          

                CPU_Queue.push(make_pair(Process[ioongoingpid].front(), ioongoingpid));
                Process[ioongoingpid].pop();
            }

            IO_Process_On = true;
        }
        if (waitprocessgoingon >= 1){
            waitprocessgoingon--;
        }

        if (IO_Wait >= 1){
            IO_Wait--;
        }

        if (IO_Process_On && IO_Wait == 0){
            if (Process[ioongoingpid].size() > 0){
                continue;
            }
            else{
                turnaroundTime[ioongoingpid] = time - ptot[ioongoingpid] + 1;
                wait_time[ioongoingpid] = turnaroundTime[ioongoingpid] - burstTime[ioongoingpid];
                penalty[ioongoingpid] = (burstTime[ioongoingpid] + wait_time[ioongoingpid]) * 1.0 / burstTime[ioongoingpid];
            }
            IO_Process_On = false;
        }

        if (process_On && waitprocessgoingon == 0)
        {

            if (Process[ongoingpid].size() > 0)
            {
                if (iocheck[ongoingpid] == 0)
                {
                    iocheck[ongoingpid] = 1;
                    IO_Queue.push(make_pair(Process[ongoingpid].front(), ongoingpid));
                    Process[ongoingpid].pop();
                }
            }
            else{
                turnaroundTime[ongoingpid] = time - ptot[ongoingpid] + 1;
                wait_time[ongoingpid] = turnaroundTime[ongoingpid] - ptime_total[ongoingpid];
                penalty[ongoingpid] = (burstTime[ongoingpid] + wait_time[ongoingpid]) * 1.0 / burstTime[ongoingpid];
            }
            tbreak = 0;
            process_On = false;

            if (CPU_Queue.empty() && IO_Queue.empty()){
                break;
            }
        }
        if (tbreak == timeslice && process_On==true){
            CPU_Queue.push(make_pair(waitprocessgoingon, ongoingpid));
            process_On = false;
            tbreak = 0;
        }
    }
    time1 = time;

    float runTime = 0, avgBurstTime = 0, avgTurnaroundTime = 0, avgWaitTime = 0, penaltyRatio = 0, sizeOfProcess = rtime_total.size();

    cout << "Process_ID  --  Response_Time  --  Wait_Time  --  Burst_Time  --  Turn_Around_Time  --  Penalty_Ratio"<<endl;

    for (int i = 0; i < sizeOfProcess; i++){        
        cout << "Process ID = " << i << " Response Time = " << rtime_total[i] << " Wait Time = " << wait_time[i] << " Burst Time = " << burstTime[i] << " Turn Around Time = " << turnaroundTime[i] << " Penalty Ratio = " << penalty[i] << endl;

        penaltyRatio += penalty[i];
        avgWaitTime += wait_time[i];
        avgTurnaroundTime += turnaroundTime[i];
        avgBurstTime += burstTime[i];
        runTime += rtime_total[i];
    }
    cout << "Response Time = " << runTime / sizeOfProcess << " Wait Time = " << avgWaitTime / sizeOfProcess << " Burst Time = " << avgBurstTime / sizeOfProcess << " Turn Around Time = " << avgTurnaroundTime / sizeOfProcess << " Penalty Ratio = " << penaltyRatio / sizeOfProcess << endl;

    cout << "System throughput = " << sizeOfProcess * 1.0 / (time1 - 1) << endl;
}

int main(int argc, char *argv[]){

    if(argc<2){
        cout<<"Please provide the input file for the program to run."<<endl;
        return 0;
    }
    string fileLine; 
    if(argc == 2){
        fileLine = argv[1];
    }

    // Not working properly
    if(argc == 3){
        // Convert the argv[2] to int
        timeslice = atoi(argv[2]);
    }

    else{
        timeslice = 3;
    } 
    readFile(fileLine);
    // Execution(fileLine);
    return 0;
}