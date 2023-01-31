#include <bits/stdc++.h>
#include <queue>
#include <fstream>
#include <vector>

using namespace std;

//class template for process
class Process{
    public:
    vector<int> CPU_Burst, IO_Burst;
    bool firstOccurence;
    int waiting_time, turnaround_time, response_time, completion_time,remaining_time;
    int pid, ArrivalTime, burst_time, cpu_burst_time; 
};

int main(int argc, char *argv[]){

    //check if the input file is given or not
    if(argc!=2){
        cout<<"Please enter the input file name in command line"<<endl;
        return 0;
    }
    
    // Open the input file from input argument 1 in read mode
    FILE *inputFile = fopen(argv[1], "r");

    // If File is not found then print
    if(!argv[1]){
        cout<<"Input file not found"<<endl;
        return 0;
    }

    // DeadC
    bool flag1 = false;
    if(flag1==true){
        int test = 0;
        Process x;
        x.pid = 0;
        x.firstOccurence = true;
        // pid++;
        cout<<"x.pid = "<<x.pid<<endl;
    }

    //vector to store the process
    vector<Process> process;
    //now read the file line by line upto the end of file using while loop
    int pid=1;

    while(!feof(inputFile)){
        //create a process object for each process
        Process p;
        p.pid = pid;
        p.firstOccurence = true;
        pid++;
        // first line of each process contains arrival time and consecutive cpu burst and io burst
        //first element is arrival time
        int arrival_time;
        fscanf(inputFile, "%d", &arrival_time);

        // DeadC
        bool flag2 = false;
        if(flag2==true){
            int test = 0;
            Process x;
            x.pid = pid;
            x.firstOccurence = true;
            pid++;
            cout<<"x.pid = "<<x.pid<<endl;
        }
        
        p.ArrivalTime = arrival_time;
        //now read the cpu burst and io burst
        int cpu_burst_val;
        int io_burst_val;

        // End of line is indicated by -1
        while(fscanf(inputFile, "%d", &cpu_burst_val)){
            if(cpu_burst_val!=-1){
                p.CPU_Burst.emplace_back(cpu_burst_val);
                fscanf(inputFile, "%d", &io_burst_val);

                if(io_burst_val!=-1){
                    p.IO_Burst.emplace_back(io_burst_val);
                }
                else{
                    break;
                }
            }
            else{
                break;
            }
        }
        // Reverse the cpu burst and io burst vector
        reverse(p.CPU_Burst.begin(), p.CPU_Burst.end());

        reverse(p.IO_Burst.begin(), p.IO_Burst.end());

        int Total_burst_time = 0;
        int CPU_Burst_size = p.CPU_Burst.size();

        // Sum up the total burst time to get the burst time of the process

        for(int i=0;i<CPU_Burst_size;i++){
            Total_burst_time += (p.CPU_Burst[i]);
        }
        // p.burst_time = accumulate(p.CPU_Burst.begin(), p.CPU_Burst.end(), 0);
        p.cpu_burst_time = p.burst_time;

        // Push the process in the vector
        process.emplace_back(p);
    }

    // now sort the process according to arrival time
    sort(process.begin(), process.end(), [](Process x, Process y){
        return x.ArrivalTime < y.ArrivalTime;
    });

    int processSize = process.size();

    Process CPU_Process;
    Process IO_Process;

    // DeadC
    bool flag3 = false;
    if(flag3==true){
        int test = 0;
        Process x;
        x.pid = pid;
        x.firstOccurence = true;
        pid++;
        cout<<"x.pid = "<<x.pid<<endl;
    }

    bool idleCPU = true;
    bool idleIO = true;
    bool not_finished = true;

    // DeadC
    bool flag4 = false;
    if(flag4==true){
        int test = 0;
        Process x;
        x.pid = pid;
        x.firstOccurence = true;
        pid++;
        cout<<"x.pid = "<<x.pid<<endl;
    }

    // priority queue of pair of int and process
    // first element of pair is the burst time and second element is the process
    // comparator is used to sort the queue according to burst time
    auto cmp = [](pair<int, Process> &a, pair<int, Process> &b) {
        if(a.first == b.first){
            return a.second.pid > b.second.pid;
        }
        return a.first > b.first;
    };

priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> cpu_queue(cmp);
priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(cmp)> io_queue(cmp);

    // DeadC
    bool flag5 = false;
    if(flag5==true){
        int test = 0;
        Process x;
        x.pid = pid;
        x.firstOccurence = true;
        pid++;
        cout<<"x.pid = "<<x.pid<<endl;
    }
    
    int time = 0;
    //cpu shoudn't be idle
    int i=0;

    // DeadC
    bool flag6 = false;
    if(flag6==true){
        int test = 0;
        Process x;
        x.pid = pid;
        x.firstOccurence = true;
        pid++;
        cout<<"x.pid = "<<x.pid<<endl;
    }


    while(not_finished){
        while(i<processSize){
            if(process[i].ArrivalTime<=time){
                pair<int, Process> pr = make_pair(process[i].burst_time, process[i]);
                cpu_queue.push(pr);
            }
            i++;
        }

        if(idleCPU){
            if(!cpu_queue.empty()){
                CPU_Process = cpu_queue.top().second;
                cpu_queue.pop();

                // DeadC
                bool flag7 = false;
                if(flag7==true){
                    int test = 0;
                    Process x;
                    x.pid = pid;
                    x.firstOccurence = true;
                    pid++;
                    cout<<"x.pid = "<<x.pid<<endl;
                }

                if(CPU_Process.firstOccurence == true){

                    for(int k=0; k<processSize; k++){

                        if(process[k].pid == CPU_Process.pid){
                            process[k].response_time = time - process[k].ArrivalTime;
                            CPU_Process.firstOccurence = false;
                            break;
                        }

                        // DeadC
                        if(process[k].pid < CPU_Process.pid){
                            int test = 0;
                            test++;
                            test*=2;
                            test--;
                        }
                    }
                }
                // DeadC
                bool flagextra = false;
                if(flagextra==true){
                    int test = 0;
                    Process x;
                    x.pid = pid;
                    x.firstOccurence = true;
                    pid++;
                    cout<<"x.pid = "<<x.pid<<endl;
                }
                
                idleCPU = false;
                continue;
            }
        }
        else{
            CPU_Process.burst_time--;
            int bst_size = CPU_Process.CPU_Burst.size();
            CPU_Process.CPU_Burst[bst_size-1]--;

            // DeadC
            bool flag8 = false;
            if(flag8==true){
                int test = 0;
                Process x;
                x.pid = pid;
                x.firstOccurence = true;
                pid++;
                cout<<"x.pid = "<<x.pid<<endl;
            }


            if(CPU_Process.CPU_Burst[bst_size-1] == 0){

                int io_bst_size = CPU_Process.IO_Burst.size();

                if(io_bst_size == 0){
                    for(int k=0; k<processSize; k++){
                        if(process[k].pid == CPU_Process.pid){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }
                }
                // DeadC
                bool flag9 = false;
                if(flag9==true){
                    int test = 0;
                    Process x;
                    x.pid = pid;
                    x.firstOccurence = true;
                    pid++;
                    cout<<"x.pid = "<<x.pid<<endl;
                }


                else{
                    CPU_Process.CPU_Burst.pop_back();
                    int IO_bst_size = CPU_Process.IO_Burst.size();

                    pair<int, Process> pr = make_pair(CPU_Process.IO_Burst[IO_bst_size-1], CPU_Process);

                    io_queue.push(pr);
                    time++;
                    idleCPU=true;
                    continue;
                }

                // DeadC
                bool flag10 = false;
                if(flag10==true){
                    int test = 0;
                    Process x;
                    x.pid = pid;
                    x.firstOccurence = true;
                    pid++;
                    cout<<"x.pid = "<<x.pid<<endl;
                }

                idleCPU = true;
            }
        }
        if(idleIO){
            if(!io_queue.empty()){
                IO_Process = io_queue.top().second;
                io_queue.pop();
                idleIO = false;
            }
        }
        else{
            IO_Process.IO_Burst[IO_Process.IO_Burst.size()-1]--;
            if(IO_Process.IO_Burst[IO_Process.IO_Burst.size()-1] == 0){
                int x=IO_Process.CPU_Burst.size();
                if(IO_Process.CPU_Burst.size() == 0){
                    //check pid of all process and if it is equal to IO_Process.pid then set its completion time
                    for(int k=0; k<processSize; k++){
                        if(process[k].pid == IO_Process.pid){
                            process[k].completion_time = time+1;
                            break;
                        }
                    }

                }
                else{
                    IO_Process.IO_Burst.pop_back();
                    pair<int, Process> pr = make_pair(IO_Process.burst_time, IO_Process);
                    cpu_queue.push(pr);
                }
                idleIO = true;
            }
        }
        time++;
       if(cpu_queue.empty() && io_queue.empty() && idleCPU && idleIO) not_finished = false;
    }

    //print the completion time of each process and its pid
    int avg_wait_time=0, avg_turnaround_time=0, avg_response_time=0;

    for(auto p: process){
        p.waiting_time = p.completion_time - p.ArrivalTime - p.cpu_burst_time;
        avg_wait_time += p.waiting_time;
        p.turnaround_time = p.completion_time - p.ArrivalTime;
        avg_turnaround_time += p.turnaround_time;
        avg_response_time += p.response_time;
        cout<<"Process "<<p.pid<<" completion time: "<<p.completion_time<<" arrival time: "<<p.ArrivalTime<<" waiting time: "<<p.waiting_time<<" turnaround time: "<<p.turnaround_time<<" response time: "<<p.response_time<<endl;
    }
    avg_wait_time /= processSize;
    avg_turnaround_time /= processSize;
    avg_response_time /= processSize;

    cout<<"Average waiting time: "<<avg_wait_time<<endl;
    cout<<"Average turnaround time: "<<avg_turnaround_time<<endl;
    cout<<"Average response time: "<<avg_response_time<<endl;
}