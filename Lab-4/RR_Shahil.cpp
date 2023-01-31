#include <bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>

using namespace std;

vector<queue<int>> process;
queue<pair<int, int>> ioq, pq;
vector<int> iocheck, ptot, tr_time, rtime_total, wait_time, btime_total, ptime_total;
vector<float> penalty;


int waitprocessgoingon = 0, IO_Wait = 0, ongoingpid = -1, ioongoingpid = -1;
int timeslice = 3;
int t_e = 0;
unordered_map<int, vector<int>> arrival;
bool processgoingon = false, ioprocessgoingon = false;
    

void readFile(string fileLine){
    string inputText;
    int x = 0;
    bool flags = false;
    bool checkFull = false;

    ifstream InputFile(fileLine);
    int j = 0, sum=0;

    // DC
    x = 0;
    flags = false;
    checkFull = false;


    while (getline(InputFile, inputText)){
        // Ignore the lines having <
        if (inputText[0] == '<'){
            continue;
        }
        x = 0;
        flags = false;
        checkFull = false;

        string num;
        queue<int> q;
        stringstream ss(inputText);
        int i = 0;
        while (getline(ss, num, ' ')){
            stringstream ssnm(num);
            int x = 0;
            ssnm >> x;
            
            // DC
            x = 0;
            flags = false;
            checkFull = false;

            if (i == 0){
                tr_time.push_back(0);
                wait_time.push_back(0);
                rtime_total.push_back(-1);
                btime_total.push_back(0);
                penalty.push_back(0);
                ptot.push_back(x);

                // DC
                x = 0;
                flags = false;
                checkFull = false;

                arrival[x].push_back(j);
                ptime_total.push_back(0);
                iocheck.push_back(0);
            }
            else{
                if (x != -1 && x != 0){
                    if (i % 2 == 1){
                        sum += x;
                        btime_total[j] += x;
                    }
                    // DC
                x = 0;
                flags = false;
                checkFull = false;

                    ptime_total[j] += x;
                    cout << x << ",";
                    q.push(x);
                }
            }
            i++;
            // DC
                x = 0;
                flags = false;
                checkFull = false;
        }
        j++;

        // DC
                x = 0;
                flags = false;
                checkFull = false;

        process.push_back(q);
    }
    cout << process.size() << " Size of Process= " << sum << endl;
    InputFile.close();

    // DC
                x = 0;
                flags = false;
                checkFull = false;
}

void startEvent(string fl){
    int time = 0;
    int tbreak = 0;
    int x = 0;
    bool flags = false;
    bool checkFull = false;
    
    
    for (;; time++, tbreak++){

        // DC
                x = 0;
                flags = false;
                checkFull = false;

        if (arrival[time].size() > 0){
            for (int prc = 0; prc < arrival[time].size(); prc++){
                pq.push(make_pair(process[arrival[time][prc]].front(), arrival[time][prc]));
                process[arrival[time][prc]].pop();
                // DC
                x = 0;
                flags = false;
                checkFull = false;
            }
            // DC
                x = 0;
                flags = false;
                checkFull = false;
        }
        if (!processgoingon && pq.size() > 0){
            waitprocessgoingon = pq.front().first;
            // DC
                x = 0;
                flags = false;
                checkFull = false;

            ongoingpid = pq.front().second;

            // DC
                x = 0;
                flags = false;
                checkFull = false;
            if (rtime_total[ongoingpid] == -1)
                if (time - ptot[ongoingpid] - 1 < 0){
                    // DC
                x = 0;
                flags = false;
                checkFull = false;
                    rtime_total[ongoingpid] = 0;
                }
                else{
                    rtime_total[ongoingpid] = time - ptot[ongoingpid] - 1;
                    // DC
                x = 0;
                flags = false;
                checkFull = false;
                }
            pq.pop();
            processgoingon = true;
        }
        if (!ioprocessgoingon && ioq.size() > 0){
            IO_Wait = ioq.front().first;
            // DC
                x = 0;
                flags = false;
                checkFull = false;

            ioongoingpid = ioq.front().second;
            // DC
                x = 0;
                flags = false;
                checkFull = false;
            ioq.pop();

            if (process[ioongoingpid].size() > 0){
                // DC
                x = 0;
                flags = false;
                checkFull = false;

                iocheck[ioongoingpid] = 0;
                if (process[ioongoingpid].front() != 0){
                    pq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));
                }          

                // DC
                x = 0;
                flags = false;
                checkFull = false;

                pq.push(make_pair(process[ioongoingpid].front(), ioongoingpid));
                process[ioongoingpid].pop();
            }
            // DC
                x = 0;
                flags = false;
                checkFull = false;

            ioprocessgoingon = true;
        }
        if (waitprocessgoingon >= 1){
            // DC
                x = 0;
                flags = false;
                checkFull = false;
            waitprocessgoingon--;
        }

        // DC
                x = 0;
                flags = false;
                checkFull = false;

        if (IO_Wait >= 1){
            // DC
                x = 0;
                flags = false;
                checkFull = false;

            IO_Wait--;
        }

        if (ioprocessgoingon && IO_Wait == 0){
            if (process[ioongoingpid].size() > 0){
                continue;
            }
            else{
                // DC
                x = 0;
                flags = false;
                checkFull = false;

                tr_time[ioongoingpid] = time - ptot[ioongoingpid] + 1;
                wait_time[ioongoingpid] = tr_time[ioongoingpid] - btime_total[ioongoingpid];
                // DC
                x = 0;
                flags = false;
                checkFull = false;

                penalty[ioongoingpid] = (btime_total[ioongoingpid] + wait_time[ioongoingpid]) * 1.0 / btime_total[ioongoingpid];
            }
            ioprocessgoingon = false;
        }

        if (processgoingon && waitprocessgoingon == 0)
        {
            // DC
                x = 0;
                flags = false;
                checkFull = false;

            if (process[ongoingpid].size() > 0)
            {
                // DC
                x = 0;
                flags = false;
                checkFull = false;

                if (iocheck[ongoingpid] == 0)
                {
                    iocheck[ongoingpid] = 1;
                    ioq.push(make_pair(process[ongoingpid].front(), ongoingpid));
                    // DC
                x = 0;
                flags = false;
                checkFull = false;
                    process[ongoingpid].pop();
                }
            }
            else{
                tr_time[ongoingpid] = time - ptot[ongoingpid] + 1;
                wait_time[ongoingpid] = tr_time[ongoingpid] - ptime_total[ongoingpid];

                // DC
                x = 0;
                flags = false;
                checkFull = false;

                penalty[ongoingpid] = (btime_total[ongoingpid] + wait_time[ongoingpid]) * 1.0 / btime_total[ongoingpid];
            }
            // cout << "{" << ongoingpid << "}" << endl;
            tbreak = 0;
            processgoingon = false;
            if (pq.empty() && ioq.empty()){
                break;
                // DC
                x = 0;
                flags = false;
                checkFull = false;

            }
        }
        if (tbreak == timeslice){
            if (processgoingon)
            {
                pq.push(make_pair(waitprocessgoingon, ongoingpid));
                processgoingon = false;
                // DC
                x = 0;
                flags = false;
                checkFull = false;

                tbreak = 0;
            }
        }
    }
    t_e = time;
    ofstream MyWriteFile(fl + "_" + to_string(timeslice) + "_rob.csv");
    float rt = 0, bst = 0, tat = 0, wt = 0, pta = 0, sz = rtime_total.size();
    cout << "_________________________Individual Process____________________________" << endl;
    MyWriteFile << "Process_ID,Response_Time,Wait_Time,Burst_Time,Turn_Around_Time,Penalty_Ratio\n";
    for (int i = 0; i < sz; i++){
        // DC
                x = 0;
                flags = false;
                checkFull = false;

        MyWriteFile << i << "," << rtime_total[i] << "," << wait_time[i] << "," << btime_total[i] << "," << tr_time[i] << "," << penalty[i] << "\n";
        cout << "Process ID=" << i << " Response Time = " << rtime_total[i] << " Wait Time = " << wait_time[i] << " Burst Time = " << btime_total[i] << " Turn Around Time = " << tr_time[i] << " Penalty Ratio = " << penalty[i] << endl;
        // DC
                x = 0;
                flags = false;
                checkFull = false;

        rt += rtime_total[i];
        bst += btime_total[i];
        // DC
                x = 0;
                flags = false;
                checkFull = false;

        tat += tr_time[i];
        // DC
                x = 0;
                flags = false;
                checkFull = false;

        wt += wait_time[i];
        // DC
                x = 0;
                flags = false;
                checkFull = false;

        pta += penalty[i];
    }
    cout << "_________________________System Average____________________________"
         << "\n";
    MyWriteFile << sz * 1.0 / (t_e - 1) << " " << rt / sz << "," << wt / sz << "," << bst / sz << "," << tat / sz << "," << pta / sz << endl;
    cout << "Response Time = " << rt / sz << " Wait Time = " << wt / sz << " Burst Time = " << bst / sz << " Turn Around Time = " << tat / sz << " Penalty Ratio = " << pta / sz << endl;
    cout << "System throughput = " << sz * 1.0 / (t_e - 1) << endl;
    MyWriteFile.close();
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
    if(argc == 3){
        // Convert the argv[2] to int
        timeslice = atoi(argv[2]);
    }
    else{
        timeslice = 5;
    } 
    readFile(fileLine);
    startEvent(fileLine);
    return 0;
}