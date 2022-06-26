#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<string>

using namespace std;

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int burst_time2;
    int complete;
    int wait_time;
    int turnaround_time;
    int age = 0;
    int timeRan = 0;
};

struct que {
    int time_quantum;
    queue<process*> q;
    int id;
};

void sort_arrival(vector<process>& v){
    sort(v.begin(), v.end(), [](process a, process b){
        return a.arrival_time < b.arrival_time;
    });
}
void filter(vector<process>& v){
    for(int i = 0; i < v.size(); i++){
        if(v[i].arrival_time < 0){
            v.erase(v.begin() + i);
        }
        if(v[i].burst_time <= 0){
            v.erase(v.begin() + i);
        }
    }
}



int main(){
    //variables
    int k;
    string filename;
    int num_queues;
    int time_q;
    int age_interval;
    float ave_wait_time = 0;
    float ave_turnaround_time = 0;

    cout << "1 for user input and 0 for use of a file: ";
    cin >> k;

    if(k == 1){
        cout << "doing this later \n"; 
    } else if (k == 0) {
        cout << "Enter file name: ";
        cin >> filename;
    } else {
        cout << "Invalid...\n";
        exit(0);
    }
    

    // retrieving user input
    cout << "Running MFQS …… \n";
    cout <<"\n";
    cout << "Enter number of queues: ";
    cin >> num_queues;
    cout << "Enter time quantum for top queue: ";
    cin >> time_q;
    cout << "Enter ageing interval: ";
    cin >> age_interval;
    cout <<"\n";
    
    //vector of process struct holding information about all of the process coming in 
    vector<process> processes;

    //read a file and store into the vector
    int counter = 0;
   
    /*
         read the file's content and format the int to 3 sections: pid, burst, arrival
    */
    string nothing;
    ifstream file (filename + ".txt");
    int a,b;
    if(file.is_open()) {
        getline(file, nothing);
        int pid, burst, arrival;
        while(file >> pid >> burst >> arrival){
            processes.push_back(process());
            processes[counter].pid = pid;
            processes[counter].burst_time = burst;
            processes[counter].burst_time2 = burst;
            processes[counter].arrival_time = arrival;
            counter++;
        }
        file.close();   
    }else{
        cout<< "Unable to open file\n";
    }

    //call function to sort the vector of processes by the arrival time
    sort_arrival(processes);
    filter(processes);

    //intializes queues with the time quantum based on the parameters
    int count = 0;
    vector<que> queues;
    while(count < num_queues-1){
        queues.push_back(que());
        queues[count].time_quantum = (time_q * pow(2, count));
        count++;
    }
    queues.push_back(que());
    queues.back().time_quantum = (queues.back().q.front()->burst_time);
    queues.back().id = 5;
    //initializes first come first serve queue
    
    /*
        if (the remaining process is greater than 0)
            the process will run when clock time == process[i] arrival time
    */
    int clock = 0;
    int procAmount = processes.size();
    bool procRunning;
    int current = 0; // index of queue 
    int number_of_processes = 0;

    /*
     while (remaining process > 0 )
        if ( clock time == process[i] arrival time )
            processes[i] will be pushed into 1st queue
            // print statement
            if ( arrival time of next process == current process arrival time )
                continue with for loop statement @ line 147            
    */
    while(procAmount > 0) {
        for(int i = 0; i < processes.size(); i++){
            if(clock == processes[i].arrival_time){
                queues.front().q.push(&processes[i]);
                cout << "Process " << processes[i].pid << ": Arrives @\t" << clock << "\n";
                if(processes[i+1].arrival_time == processes[i].arrival_time){
                    continue;
                }else break;
            } 
        }
        
        /*
            if ( position of queue[i] is not the final queue )
                if ( 1st process at last queue with an age < age interval )
                    increment the first process @ last position by 1
                    last process of final queue will be push the the 1st position of last queue
                    front process (of final queue) will exit the queue
                    
                if ( 1st process at last queue with an age == age interval )
                    queue @ ( 2 queue before the final one ) push the ( front process of final queue ) 
                    and pop the process at the final queue
                    
                    NOTE: line 176
                    
        */  
        if(queues[current].id != 5){
            for(int i = 0; i < queues.back().q.size(); i++){
                if(queues.back().q.front()->age < age_interval){
                    queues.back().q.front()->age++;
                    //cout << queues.back().q.front()->pid << " " << queues.back().q.front()->age <<  endl;
                    queues.back().q.push(queues.back().q.front());
                    queues.back().q.pop();
                }
                if(queues.back().q.front()->age == age_interval){
                    queues[num_queues-2].q.push(queues.back().q.front());
                    queues.back().q.pop();
                }
            }
        }
        
        /*
            if (the process is still running) AND (current position of queue is not @ final) AND ( process at current queue running duration is < current queue time quantum )
                burst time of front process @ current queue will decrement by 1
                running duration of a process increment by 1
            else if ( process @ current queue reach the limit and it is still running )
                burst time of front process at current queue will decrement by 1
        */
        if(procRunning == true && queues[current].id != 5 && queues[current].q.front()->timeRan < queues[current].time_quantum){
            queues[current].q.front()->burst_time--;
            queues[current].q.front()->timeRan++;
        }else if(queues[current].id == 5 && procRunning == true){
            queues[current].q.front()->burst_time--;
        } 

        for(int j = 0; j<num_queues; j++){
            if(!queues[j].q.empty()){
                if(j == (current - 1)){
                    procRunning = false;
                    current = j;
                    break;
                } else {
                    current = j;
                    break;
                }
            }
        }
        

        //checks is the current queue is empty and a process isn't running
        //if true then print process runs at the current clock
        //set procRunning equal to true
        if(queues[current].q.size() > 0 && procRunning == false){
            cout << "Process " << queues[current].q.front()->pid << ": Runs @\t" << clock <<"\n";
            procRunning = true;
        }
        //if the burst time of the front of the current queue hits zero and procRunning is true then
        //print that a process finishes at the clock
        //set the complete time to the clock
        //increment the number of processes completed, decrement the amount of processes left
        //pop from the current queue
        //set procRunning to false
        /*
            if ( current queue is not empty ) OR ( next queue is not empty ) AND ( no process is running )
                if ( current queue is not empty )
                    process at current queue will be running
                else if ( current queue is empty )
                    process will run at current queue when it hits the arrival time
                    index of current will increment by 1
            
        */
    
        if(queues[current].q.front()->burst_time == 0 && procRunning) {
                cout << "Process " << (queues[current].q.front()->pid) << ": Finished @\t" << clock << "\n";
                queues[current].q.front()->complete = clock;
                number_of_processes++;
                procAmount--;
                queues[current].q.pop();
                procRunning = false; 
                if(!queues[current].q.empty() || !queues[current+1].q.empty() && !procRunning){
                    if(!queues[current].q.empty()){
                        cout << "Process " << queues[current].q.front()->pid << ": Runs @\t" << clock << "\n";
                        procRunning = true;
                    } else if(queues[current].q.empty()){
                    cout << "Process " << queues[current+1].q.front()->pid << ": Runs @\t" << clock << "\n";
                    procRunning = true;
                    current = current + 1;
                    }
                }
        } else if(queues[current].q.front()->timeRan == queues[current].time_quantum && queues[current].id != 5 && procRunning){
            cout << "Process " << queues[current].q.front()->pid << ": Switched @\t" << clock << "\n";
            queues[current].q.front()->timeRan = 0;
            queues[current+1].q.push(queues[current].q.front()); 
            queues[current].q.pop();
            procRunning = false;
            if(!queues[current].q.empty() || !queues[current+1].q.empty() && !procRunning){
                if(!queues[current].q.empty()){
                    cout << "Process " << queues[current].q.front()->pid << ": Runs @\t" << clock << "\n";
                    procRunning = true;
                } else if(queues[current].q.empty()){
                    cout << "Process " << queues[current+1].q.front()->pid << ": Runs @\t" << clock << "\n";
                    procRunning = true;
                    current = current + 1;
                }
            }
        }

         
        
        
        clock++;
    }



    float total_turnaround = 0;
    float total_wait = 0;
    for(int l=0; l < processes.size(); l++) {
        processes[l].turnaround_time = (processes[l].complete - processes[l].arrival_time);
        processes[l].wait_time = (processes[l].turnaround_time - processes[l].burst_time2);
        total_turnaround = total_turnaround + processes[l].turnaround_time;
        total_wait = total_wait + processes[l].wait_time;
    }

    ave_turnaround_time = total_turnaround/(float)number_of_processes;
    ave_wait_time = total_wait/(float)number_of_processes;
    cout << "Ave. wait time = " << ave_wait_time << "\t" << "Ave. turnaround time = " << ave_turnaround_time << "\n";
    cout << "Total Processes Scheduled = " << number_of_processes <<  "\n";

    return 0;
}
