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
    int complete;
    int wait_time;
    int turnaround_time;
};

struct que {
    int time_quantum;
    queue<process> q;
    int id;
};

/* sorts the processes arrival time chronologically */
void sort_arrival(vector<process>& v){
    sort(v.begin(), v.end(), [](process a, process b){
        return a.arrival_time < b.arrival_time;
    });
}

/* (vector of processes) will exclude the processes with -arrival time or -burst time */
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
    /* crated 3 variables to accept the integers from user input/file and section them into 3 columns  */
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
    queues.back().time_quantum = (queues.back().q.front().burst_time);
    queues.back().id = 5;
    //initializes first come first serve queue
    

    int clock = 0;
    int procAmount = processes.size();
    bool procRunning;
    int processCounter = 0; // NOTE: number of process finished || waiting time of a process || count the number of process
    int current = 0;
    int number_of_processes = 0; // number of process finished
    

    while(procAmount > 0) {
        for(int i = 0; i < processes.size(); i++){
            if(clock == processes[i].arrival_time){
                queues.front().q.push(processes[i]);
                cout << "Process " + to_string(processes[i].pid) + ": Arrives @\t" + to_string(clock) +"\n";
                queues[current].q.front().complete = -2;
                if(processes[i+1].arrival_time == processes[i].arrival_time){
                    continue;
                }else break;
            } 
        }
        for(int j = 0; j<queues.size(); j++){
            if(!queues[j].q.empty()){
                current = j;
                break;
            }
        }

        if(queues[current].id == 5 && procRunning == true){
            clock = clock + queues[current].q.front().burst_time-1;
            queues[current].q.front().burst_time = 0;
        }
        /*
            if (process is still running) AND (processCounter < time quantum of current queue ) AND (position of queue is not the last)
                burst time of current queue decrement
                number of number of completed process increments    
        */

        if(procRunning == true && processCounter < queues[current].time_quantum && queues[current].id != 5){
            queues[current].q.front().burst_time--;
            processCounter++;
            cout<<" line 163 processCounter = "<<processCounter<<endl;
        }

        /*
            if (current queue is not empty) OR (next queue is not empty)
                if (no process is running)
                    the process will run when ----> clock time == process arrival time

                if (process is not running) AND (current queue is empty)
                *** NOTE: description of line 175 && 180
                       
        */

        if(!queues[current].q.empty() || !queues[current+1].q.empty()){
            if(procRunning != true){
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                processCounter = 0;
                procRunning = true;
            }
            if(procRunning != true && queues[current].q.empty()){   
                cout << "Process " + to_string(queues[current+1].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                processCounter = 0;
                procRunning = true;
            }
        }

        /*
            if ( current queue with burst time == 0 ) AND ( there is a process running in a queue ) 
                it means the process has completed its burst time
                number of completed process incremented
                the completed process will exit queue
        */
        
        if(queues[current].q.front().burst_time == 0 && procRunning == true) {
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Finished @\t" + to_string(clock) +"\n";
                //queues[current].q.front().complete = clock;
                cout << queues[current].q.front().complete << endl;
                number_of_processes++;
                procAmount--;
                queues[current].q.pop();
                procRunning = false;
                processCounter = 0;

                /*
                    if (current queue is not empty) OR (next queue is not empty) OR (number of processes completed < total processes from user input )
                        if (a process in a queue is not running) AND (current queue is not empty)
                            a process will run when clock time == process arrival time 
                            ** NOTE:  process counter
                            
                */
                
            if(!queues[current].q.empty() || !queues[current+1].q.empty() && number_of_processes < processes.size()){
                if(procRunning == false && !queues[current].q.empty()){
                    cout << "Process " + to_string(queues[current].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                    processCounter = 0;
                    procRunning = true;
                }

                /*
                    if (the process is not running ) AND (current queue is empty)
                        a process will run when clock time == process arrival time 
                        
                */
                if(procRunning == false && queues[current].q.empty()){
                    cout << "Process " + to_string(queues[current+1].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                    processCounter = 0;
                    procRunning = true;
                }
            }
        }
            /*
                NOTE: processCounter 
            */
        
        if(processCounter == queues[current].time_quantum && !queues[current].q.empty() && queues[current].id != 5){
            cout << "Process " + to_string(queues[current].q.front().pid) + ": Switched @\t" + to_string(clock) +"\n";
            queues[current+1].q.push(queues[current].q.front()); 
            queues[current].q.pop();
            procRunning = false;

            /*
                if (current queue is not empty) OR (next queue is not empty)
                    process at current queue will run when clock time == process arrival time

            */
            if(!queues[current].q.empty() || !queues[current+1].q.empty()){
                if(procRunning != true && !queues[current].q.empty()){
                    cout << "Process " + to_string(queues[current].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                    processCounter = 0;
                    procRunning = true;
                }
                /*
                    if (no process is running) AND ( current queue is empty)
                        process at current queue will run when clock time == process arrival time
                */
                if(procRunning != true && queues[current].q.empty()){
                    cout << "Process " + to_string(queues[current+1].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                    processCounter = 0;
                    procRunning = true;
                }   
            }
        }
        clock++;
    }



    float total_turnaround = 0;
    float total_wait = 0;
    for(int l=0; l < processes.size(); l++){
        cout << processes[l].complete;
        //processes[l].turnaround_time = (processes[l].complete - processes[l].arrival_time);
        //processes[l].wait_time = (processes[l].turnaround_time - processes[l].burst_time);
        //cout << processes[k].turnaround_time << " " << processes[k].wait_time;
        
        //total_turnaround = total_turnaround + processes[l].turnaround_time;
        //total_wait = total_wait + processes[l].wait_time;
    }
    //cout<<"line 227 total turnaround "<< total_turnaround<<endl;
    //cout<<"line 228 total wait "<<total_wait<<endl;

    ave_turnaround_time = total_turnaround/(float)number_of_processes;
    ave_wait_time = total_wait/(float)number_of_processes;
    cout << "Ave. wait time = " + to_string(ave_wait_time) << "\t" << "Ave. turnaround time = " + to_string(ave_turnaround_time) + "\n";
    cout << "Total Processes Scheduled = " + to_string(number_of_processes) + "\n";

    return 0;
}
