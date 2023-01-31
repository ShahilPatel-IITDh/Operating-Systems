#include <bits/stdc++.h>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

// We make a class for process, so that we can use it as a template to define various proccesses and the their attributes such as arrival time, burst time, turnaround time, etc.

class Process{
    public:
        int arrivalTime, process_burstTime, CPU_burstTime, pid;
        vector<int> IO_burst, CPU_burst;
        int CPU_burstSize, IO_burstSize, waitingTime, turnaroundTime, responseTime, completionTime, timeRemaining;
        bool firstOccurence; 
};

int main(int argc, char *argv[]){
    // Now we open the file mentioned in argument 1 in read mode
    FILE *inputFile = fopen(argv[1], "r");

    // FILE* inputFile = fopen(argv[1], "r");

    // If the argument has no input file then print that file not provided in argument
    if(inputFile == NULL){
        cout<<"The Input File is not provided in argument!"<<endl;
        exit(0);
    }

    // Now make a vector of type process to store all the process along with attributes of each process
    // Here we use user defined type 'process' because all the entities in the vector will contain the attributes of the process
    vector<Process> process;

    // The input is from argument file, so we need to read the file till it's END;
    int pid = 1;

    // feof will return 0 if the reader has not yet reached the end of the file
    while(!feof(inputFile)){
        // Object declaration
        Process a;
        a.pid = pid;
        // as the process is occuring for the first time, mark its first occurence as true
        a.firstOccurence = true;
        pid++;

        // Read the file line by line and store each line in a string called data;
        // string data;
        // getline(inputFile, data);
        // As the string will contain data in char type, we convert it into int type
        int arrivalTime;
        fscanf(inputFile, "%d", &arrivalTime);
        a.arrivalTime = arrivalTime;
        
        // Now we need to read the CPU and IO bursts
        int IO_burst, CPU_burst;

        // Our end of line is mentioned using -1
        int length = data.length();
        int i=1;
        while(fscanf(inputFile, "%d", &CPU_burst) && CPU_burst != -1){
            a.CPU_burst.push_back(CPU_burst);
            fscanf(fp, "%d", &io_burst);
            if(io_burst!=-1){
                a.IO_burst.push_back(IO_burst);
            }
            else{
                break;
            }
        }

            // Now we need to reverse the CPU and IO burst vectors
            
        reverse(a.IO_burst.begin(), a.IO_burst.end());
        reverse(a.CPU_burst.begin(), a.CPU_burst.end());
        int total_burst = 0;
        int burstLength = a.CPU_burst.size();
        for(auto x: a.CPU_burst){
            total_burst+=x;
        }
        a.process_burstTime = total_burst;
        a.CPU_burstTime = total_burst;

        // As the process a is now assigned all the attributes, push it to the vector of processes
        process.emplace_back(a);
    }

        // Our algorithm is Shortest job first, so sort the process according to their arrival time
        sort(process.begin(), process.end());

        int numberOfProcesses = process.size();

         sort(process.begin(), process.end(), [](Process a, Process b){
            return a.arrival_time < b.arrival_time;
        });

        Process CPU_Process;
        Process IO_Process;

        bool idleCPU = true;
        bool idleIO = true;
        bool notFinished = true;

        // priority Queue of pair of int and process, where the int is used to define burst time, we use comparator to sort the queue according to burst time.

        auto compareProcess = [](pair<int, Process> &a, pair<int, Process> &b){
            // If two processes have equal burst time then prioritze the process having lower PID
            if(a.first == b.first){
                return (a.second.pid > b.second.pid);
            }

            // Else prioritze the process with shortest burstime
            // else if(a.first != b.first){
            //     return (a.first>b.first);
            // }
            return a.first>b.first;
        };

        priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(compareProcess)> CPUQueue(compareProcess);

        priority_queue<pair<int, Process>, vector<pair<int, Process>>, decltype(compareProcess)> IOQueue(compareProcess);

        int time = 0;
        // We have to avoid making CPU idle

        int j = 0;
        while(notFinished){
            while(j<n && process[j].arrivalTime <= time){
                CPUQueue.push({process[j].process_burstTime, process[j]});
                j++;
            }
            // If the CPU is idle
            if(idleCPU){
                if(!CPUQueue.empty()){
                    // If CPU is idle and the CPU queue is not yet empty then assign the first process from the queue
                    CPU_Process = CPUQueue.front().second;
                    CPUQueue.pop();
                    
                    // Check if the process is occuring for the first time.
                    if(CPU_Process.firstOccurence){
                        for(int k=0; k<numberOfProcesses; k++){

                            // If the PID of kth process is equal to our current CPU process, it means that the process has already occured before

                            if(process[k].pid == CPU_Process.pid){
                                process[k].responseTime = time-process[k].arrivalTime;
                                CPU_Process.firstOccurence = true;
                            }
                        }
                    }
                    idleCPU = false;
                    continue;
                }
            }
            else{
                CPU_Process.process_burstTime--;
                CPU_Process.CPU_burst[CPU_Process.CPU_burst.size()-1]--;
                
                if(CPU_Process.CPU_burst[CPU_Process.CPU_burst.size()-1] == 0){
                    if(CPU_Process.IO_burst.size() == 0){
                        for(int k=0; k<numberOfProcesses; k++){
                            if(process[k].pid == CPU_Process.pid){
                                process[k].completionTime = time+1;
                                break;
                            }
                        }
                    }
                    else{

                        // Pop the last process from CPU Queue
                        CPU_Process.CPU_burst.pop_back();
                        IOQueue.push({CPU_Process.IO_burst[CPU_Process.IO_burst.size()-1], CPU_Process});
                        time++;
                        idleCPU = true;
                        continue;
                    }
                    idleCPU = true;
                }
            }
            if(idleIO){
                if(!IOQueue.empty()){
                    idleIO = false;
                    IO_Process = IOQueue.front().second;
                    IOQueue.pop();
                }
            }
            else if(!idleIO){
                IO_Process.IO_burst[IO_Process.IO_burst.size() - 1]--;
                if(IO_Process.IO_burst[IO_Process.IO_burst.size()-1] == 0){
                    int temp = IO_Process.CPU_burst.size();
                    if(IO_Process.CPU_burst.size() == 0){
                        // Check PID of all process and if it equals current process's PID then set its completion time
                        for(int k=0; k<numberOfProcesses; k++){
                            if(process[k].pid == IO_Process.pid){
                                process[k].completionTime = time+1;
                                break;
                            }
                        }
                    }
                    else if(IO_Process.CPU_burst.size() != 0){
                        IO_Process.IO_burst.pop_back();
                        CPUQueue.push({IO_Process.process_burstTime, IO_Process});
                    }
                    idleIO = true;
                }
            }
            time++;
            if(CPUQueue.empty() && IOQueue.empty() && idleCPU && idleIO){
                notFinished = false;
            }
        }
        // Printing the required output
        int avgWaitingTime = 0;
        int avgTurnaroundTime = 0;
        int avgResponseTime = 0;
        int totalWaitTime = 0;
        int totalTurnaroundTime = 0;
        int totalResponseTime = 0;
        for(auto a: process){
            a.waitingTime = a.completionTime - (a.arrivalTime + a.CPU_burstTime);
            totalWaitTime += a.waitingTime;
            a.turnaroundTime = (a.completionTime - a.arrivalTime);
            totalTurnaroundTime += a.turnaroundTime;
            totalResponseTime += a.responseTime;
            cout<<"Process:   "<<a.pid<<"   Completion Time:   "<<a.completionTime<<"   Arrival Time:   "<<a.arrivalTime<<"   Waiting Time:   "<<a.waitingTime<<"   Turnaround Time:   "<<a.turnaroundTime<<"   Response Time:   "<<a.responseTime<<endl;
        }
        avgResponseTime = (totalResponseTime/numberOfProcesses);
        avgTurnaroundTime = (totalTurnaroundTime/numberOfProcesses);
        avgWaitingTime = (totalWaitTime/numberOfProcesses);

        cout<<"Average Waiting Time is:   "<<avgWaitingTime<<endl;
        cout<<"Average Response Time is:  "<<avgResponseTime<<endl;
        cout<<"Average Turnaround Time is "<<avgTurnaroundTime<<endl;

    inputFile.close();
}
