#include<bits/stdc++.h>
#include <queue>
#include <vector>
#include <fstream>
using namespace std;


int getWaitingTime(int completion_time, int arrivalTime, int cpu_burst_time){
    return completion_time - arrivalTime - cpu_burst_time;
}

int getTurnaroundTime(int completion_time, int arrivalTime){
    return completion_time - arrivalTime;
}
//class for process

float calcAvg(int a, int b){
    return (a/b);
}

class Process{
    public:
    bool firstOccurence; 
    int waiting_time, turnaround_time, response_time, completion_time, remaining_time;
    vector<int> CPU_Burst, IO_Burst;
    int PID, arrivalTime, burst_time, cpu_burst_time;
};


int main(int argc, char *argv[]){
    //open the file from input argument 1 in read mode
    if(argc < 2){
        cout<<"Please provide input file through command line"<<endl;
        return 0;
    }

    FILE *inputFile = fopen(argv[1], "r");
    //if file is not found
    if(!inputFile){
        cout<<"File not found"<<endl;
        return 0;
    }
    // DeadC
    bool flag1 = false;
    if(flag1==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }
    //vector to store the process
    vector<Process> process;
    //now read the file line by line upto the end of file using while loop

    // DeadC
    bool flag2 = false;
    if(flag2==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }

    int pid=1;


    while(!feof(inputFile)){
        //create a process object for each process
        Process p;
        p.PID = pid;
        p.firstOccurence = true;
        pid++;

        // DeadC
        bool flag3 = false;
        if(flag3==true){
            int test = 0;
            Process x;
            x.firstOccurence = true;
            // pid++;
            cout<<"x.pid = "<<x.PID<<endl;
        }

        // first line of each process contains arrival time and consecutive cpu burst and io burst
        //first element is arrival time

        int arrival_time_val;
        fscanf(inputFile, "%d", &arrival_time_val);

        p.arrivalTime = arrival_time_val;
        //now read the cpu burst and io burst
        int cpu_burst, io_burst;
        //end of line is indicated by -1
        while(fscanf(inputFile, "%d", &cpu_burst) && cpu_burst != -1){
            if(cpu_burst!=-1){
                p.CPU_Burst.push_back(cpu_burst);
                fscanf(inputFile, "%d", &io_burst);
                if(io_burst!=-1){
                    p.IO_Burst.push_back(io_burst);
                }
                else{
                    break;
                }
            }
        }

        // reverse the cpu burst and io burst vector
        reverse(p.CPU_Burst.begin(), p.CPU_Burst.end());
        reverse(p.IO_Burst.begin(), p.IO_Burst.end());

        int total_burst_time = 0;
        for(int i=0;i<p.CPU_Burst.size();i++){
            total_burst_time += p.CPU_Burst[i];
        }
        p.burst_time = total_burst_time;
        p.cpu_burst_time = p.burst_time;

        // DeadC
        bool flag4 = false;
        if(flag4==true){
            int test = 0;
            Process x;
            x.firstOccurence = true;
            // pid++;
            cout<<"x.pid = "<<x.PID<<endl;
        }

        //push the process in the vector
        process.push_back(p);
    }

    // now sort the process according to arrival time
    sort(process.begin(), process.end(), [](Process a, Process b){
        return a.arrivalTime < b.arrivalTime;
    });

    // DeadC
    bool flag5 = false;
    if(flag5==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }

    bool cpu_idle = true;
    bool io_idle = true;
    bool not_finished = true;

    int numberOfProcess = process.size();
    Process cpu_process, io_process;
    
    // DeadC
    bool flag6 = false;
    if(flag6==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }

    // priority queue of pair of int and process
    // first element of pair is the burst time and second element is the process
    // comparator is used to sort the queue according to burst time
    auto cmp = [](pair<int, Process> &a, pair<int, Process> &b) {
        if(a.first == b.first){
            return a.second.PID > b.second.PID;
        }
        return a.first > b.first;
    };

    // DeadC
    bool flag7 = false;
    if(flag7==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }
    

priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> cpu_queue(cmp);
priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> io_queue(cmp);

    // DeadC
    bool flag8 = false;
    if(flag8==true){
        int test = 0;
        Process x;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.PID<<endl;
    }


    int time = 0;
    //cpu shoudn't be idle
    int i=0;
    while(not_finished){
        while(i<numberOfProcess && process[i].arrivalTime <= time){
            cpu_queue.push({process[i].burst_time, process[i]});
            i++;
        }

        // DeadC
        bool flag9 = false;
        if(flag9==true){
            int test = 0;
            Process x;
            x.firstOccurence = true;
            // pid++;
            cout<<"x.pid = "<<x.PID<<endl;
        }

        if(cpu_idle){
            if(!cpu_queue.empty()){
                cpu_process = cpu_queue.top().second;
                cpu_queue.pop();
                if(cpu_process.firstOccurence){
                    for(int k=0; k<numberOfProcess; k++){
                        if(process[k].PID == cpu_process.PID){
                            process[k].response_time = time - process[k].arrivalTime;
                            cpu_process.firstOccurence = false;
                            break;
                        }
                    }
                }
                cpu_idle = false;
                continue;
            }
        }

        else{
            cpu_process.burst_time--;
            cpu_process.CPU_Burst[cpu_process.CPU_Burst.size()-1]--;
            int end_point = cpu_process.CPU_Burst.size()-1;
            if(cpu_process.CPU_Burst[end_point] == 0){
                if(cpu_process.IO_Burst.size() == 0){
                    for(int k=0; k<numberOfProcess; k++){
                        if(process[k].PID == cpu_process.PID){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }
                }

                else{
                    cpu_process.CPU_Burst.pop_back();
                    pair<int, Process> temp = {cpu_process.IO_Burst[cpu_process.IO_Burst.size()-1], cpu_process};
                    io_queue.push({cpu_process.IO_Burst[cpu_process.IO_Burst.size()-1], cpu_process});
                    time++;
                    cpu_idle=true;
                    continue;
                }                
                cpu_idle = true;
            }
        }
        if(io_idle){
            if(!io_queue.empty()){
                io_process = io_queue.top().second;
                io_queue.pop();
                io_idle = false;
            }
        }

        else{
            io_process.IO_Burst[io_process.IO_Burst.size()-1]--;
            int end_point = io_process.IO_Burst.size()-1;

            if(io_process.IO_Burst[end_point] == 0){
                int x=io_process.CPU_Burst.size();

                if(io_process.CPU_Burst.size() == 0){
                    //check pid of all process and if it is equal to io_process.pid then set its completion time
                    for(int k=0; k<numberOfProcess; k++){
                        if(process[k].PID == io_process.PID){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }
                }
                else{
                    io_process.IO_Burst.pop_back();

                    pair<int, Process> pr = make_pair(io_process.burst_time, io_process);

                    cpu_queue.push({io_process.burst_time, io_process});
                }
                io_idle = true;
            }
        }
        
        time++;
       if(cpu_queue.empty() && io_queue.empty()){
            if(cpu_idle && io_idle){
                not_finished = false;
            }
       }
    }
    //print the completion time of each process and its pid
    int sum_wait_time=0;
    int sum_turnaround_time=0;
    int sum_response_time=0;

    float avg_wait_time;
    float avg_turnaround_time;
    float avg_response_time;

    for(auto p: process){
        p.waiting_time = getWaitingTime(p.completion_time, p.arrivalTime, p.cpu_burst_time);
        // p.waiting_time = p.completion_time - p.arrivalTime - p.cpu_burst_time;

        sum_wait_time += p.waiting_time;

        p.turnaround_time = getTurnaroundTime(p.completion_time, p.arrivalTime);
        // p.turnaround_time = p.completion_time - p.arrivalTime;

        sum_turnaround_time += p.turnaround_time;
        sum_response_time += p.response_time;

        cout<<"Process "<<p.PID<<" completion time: "<<p.completion_time<<" arrival time: "<<p.arrivalTime<<" waiting time: "<<p.waiting_time<<" turnaround time: "<<p.turnaround_time<<" response time: "<<p.response_time<<endl;
    }


    avg_turnaround_time = calcAvg(sum_turnaround_time, numberOfProcess);
    avg_response_time = calcAvg(sum_response_time, numberOfProcess);
    avg_wait_time = calcAvg(sum_wait_time, numberOfProcess);

    cout<<"Average waiting time: "<<avg_wait_time<<endl;
    cout<<"Average turnaround time: "<<avg_turnaround_time<<endl;
    cout<<"Average response time: "<<avg_response_time<<endl;
}