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
    deque<process*> q;
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
    
    cout << "Enter file name: ";
    cin >> filename;

    // retrieving user input
    cout << "Running MFQS ..... \n";
    cout <<"\n";
    while(num_queues < 2 || num_queues > 5){
        cout << "Enter number of queues: ";
        cin >> num_queues;
        if(num_queues < 2 || num_queues > 5){
            cout << "Range of queue (2 <= x <= 5) Re-enter..." << endl;
        }
    }
    while(time_q < 1){
        cout << "Enter time quantum for top queue: ";
        cin >> time_q;
        if(time_q < 1){
            cout << "Enter number greater than 0..." << endl;
        }
    }
    while(age_interval < 1){
        cout << "Enter ageing interval: ";
        cin >> age_interval;
        if(age_interval < 1){
            cout << "Enter number greater than 0..." << endl;
        }
    }

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
        while(file >> pid >> burst >> arrival>> a >> b){
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
        queues[count].time_quantum = time_q;
        time_q = time_q * 2;
        count++;
    }
    queues.push_back(que());
    queues.back().id = 5;
    //initializes first come first serve queue
    
    /*
        if (the remaining process is greater than 0)
            the process will run when clock time == process[i] arrival time
    */
    int clock = 0;
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
    while(number_of_processes < processes.size()) {

        for(int i = 0; i < processes.size(); i++){
            if(clock == processes[i].arrival_time){
                queues.front().q.push_back(&processes[i]);
                cout << "Process " << processes[i].pid << ": Arrives @\t" << clock << "\n";
                if(processes[i+1].arrival_time == processes[i].arrival_time){
                    continue;
                }else break;
            } 
        }
    
        if(queues[current].id != 5){
            for(int i = 0; i < queues.back().q.size(); i++){
                if(queues.back().q.front()->age < age_interval){
                    queues.back().q.front()->age++;
                    //cout << queues.back().q.front()->pid << " " << queues.back().q.front()->age <<  endl;
                    queues.back().q.push_back(queues.back().q.front());
                    queues.back().q.pop_front();
                }
                if(queues.back().q.front()->age == age_interval){
                    queues[num_queues-2].q.push_back(queues.back().q.front());
                    queues.back().q.pop_front();
                }
            }
        }
        
        if(procRunning == true && queues[current].id != 5 && queues[current].q.front()->timeRan < queues[current].time_quantum){
            queues[current].q.front()->burst_time = queues[current].q.front()->burst_time - 1;
            queues[current].q.front()->timeRan++;
        }else if(queues[current].id == 5 && procRunning == true){
            queues[current].q.front()->burst_time--;
        } 

        for(int j = 0; j<num_queues; j++){
            if(!queues[j].q.empty()){
                if(j == (current - 1)){
                    if(queues[current].q.front()->burst_time == 0){
                        cout << "Process " << (queues[current].q.front()->pid) << ": Finished @\t" << clock << "\n";
                        queues[current].q.front()->complete = clock;
                        number_of_processes++;
                        queues[current].q.pop_back();
                    } else if(queues[current].q.front()->timeRan == queues[current].time_quantum){
                        cout << "Process " << queues[current].q.front()->pid << ": Switched @\t" << clock << "\n";
                        queues[current].q.front()->timeRan = 0;
                        queues[current+1].q.push_back(queues[current].q.front()); 
                        queues[current].q.pop_front();
                    }
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
    
        if(queues[current].q.front()->burst_time == 0) {
                cout << "Process " << (queues[current].q.front()->pid) << ": Finished @\t" << clock << "\n";
                queues[current].q.front()->complete = clock;
                queues[current].q.pop_front();
                number_of_processes++;
                procRunning = false; 
                
                if(!queues[current].q.empty() || !queues[current+1].q.empty()){
                    if(!queues[current].q.empty()){
                        cout << "Process " << queues[current].q.front()->pid << ": Runs @\t" << clock << "\n";
                        procRunning = true;
                    } else if(queues[current].q.empty()){
                    cout << "Process " << queues[current+1].q.front()->pid << ": Runs @\t" << clock << "\n";
                    procRunning = true;
                    current = current + 1;
                    }
                }
        } else  
        if(queues[current].q.front()->timeRan == queues[current].time_quantum && queues[current].id !=5){
            cout << "Process " << queues[current].q.front()->pid << ": Switched @\t" << clock << "\n";
            queues[current].q.front()->timeRan = 0;
            queues[current+1].q.push_back(queues[current].q.front()); 
            queues[current].q.pop_front();
            procRunning = false;

            if(!queues[current].q.empty() || !queues[current+1].q.empty()){
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
