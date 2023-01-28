#include<stdio.h>

int main(){
    // ask the user to enter the number of processes and store it in n. We then accept the burst times from the user. It is stored in the burstTime array.

    int burstTime[20];
    // Burst Time values will be sorted using 'Merge sort' algorithm

    int p[20];
    int waitingTime[20];
    int TurnAroundTime[20],i,j,n,total=0,pos;

    float avg_wt,avg_tat;

    printf("Enter number of process:");
    scanf("%d",&n);
  
    printf("nEnter Burst Time:n");
    for(i=0;i<n;i++){
        printf("p%d:",i+1);
        scanf("%d",&burstTime[i]);
        p[i]=i+1;         
    }
    int temp;
    
    //sorting of burst times
    for(i=0;i<n;i++){
        pos=i;
        for(j=i+1;j<n;j++){
            if(burstTime[j]<burstTime[pos]){
                pos = j;
            }
        }
        temp=burstTime[i];
        burstTime[i]=burstTime[pos];
        burstTime[pos]=temp;
  
        temp=p[i];
        p[i]=p[pos];
        p[pos]=temp;
    }
   
    waitingTime[0]=0;            
  
    // The first element has no waiting time, and the remaining waiting time is determined by using two for loops. One for loop controls the outer loop and runs from 1 to in, while the other for loop controls the inner loop and runs from j=0 to j<i. The waiting time is calculated inside the loop by multiplying it by the burst time.

    for(i=1;i<n;i++){
        waitingTime[i]=0;
        for(j=0;j<i;j++){
            waitingTime[i]+=burstTime[j];
        }  
        total+=waitingTime[i];
    }
  
    avg_wt=(float)total/n;      
    total=0;
  
    printf("Process     Burst Time   Waiting Time   Turnaround Time");
    for(i=0;i<n;i++){
        TurnAroundTime[i]=burstTime[i]+waitingTime[i];   
        total+=TurnAroundTime[i];
        printf("np%dtt  %dtt    %dttt%d",p[i],burstTime[i],waitingTime[i],TurnAroundTime[i]);
    }
  
    avg_tat=(float)total/n;    
    printf("Average Waiting Time = %f\n",avg_wt);
    printf("Average Turnaround Time = %f\n",avg_tat);
}