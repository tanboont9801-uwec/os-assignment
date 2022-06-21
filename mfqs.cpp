#include<iostream>
#include <queue>
// might not need deque
#include <deque>
#include <vector>
using namespace std;

/*
 int pid[]
 int burst 
 int arrival

 // --- below will not be shown in the format --- 
 int priority
 int ageIndex

 int remaining_burst_time[]
 int waiting_time[]

**/
// undeclared size of queue (user input)
class mfqs{

    public:

        std::vector<int> vec;
        int time_quantum;
        int ageing_interval;
        int burst_time;
        int index;
        int queue_level;
        int temp;

        
 

    // index will be generated when the user input the process 
    void schedule(int process_id[], int index){

        // if amount of process enter queue < size of queue then enqueue the process
        for(index = 0; index < q.size(); index++){
            if(process_id[index] <= q.size()){
            vec.insert(process_id[index]);
            
              if(process_id[index].burst_time <= time_quantum){
                // process exit queue
                vec.pop(process_id[index]);
            }

            


        }
        /*
        last queue will be FCFS 
         swapping the position of the process according to the process_id 
        
        */
       for(int k = index; k < q.size()-1; k++){
        for(int j = k+1; j < q.size(); j++ ){
            if(process_id[k].burst_time > process_id[j].burst_time ){
                temp = process[k]; 
                process[k] = process[j]; 
                process[j] = temp; 
            }
            
        }
       }
        
            




    }


}