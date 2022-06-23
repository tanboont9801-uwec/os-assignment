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
    int wait_time;
    int turnaround_time;
};

struct que {
    int time_quantum;
    queue<process> q;
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
    int number_of_processes;
    double ave_wait_time;
    double ave_turnaround_time;

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
    //queue<process> fcfs;
    

    int clock = 0;
    int procAmount = processes.size();
    bool procRunning;
    int processCounter = 0;
    int current = 0;
    
    while(procAmount > 0) {
        for(int i = 0; i < processes.size(); i++){
            if(clock == processes[i].arrival_time){
                queues.front().q.push(processes[i]);
                cout << "Process " + to_string(processes[i].pid) + ": Arrives @\t" + to_string(clock) +"\n";
                number_of_processes = number_of_processes + 1;
            }
        }
        for(int j = 0; j<queues.size(); j++){
            if(!queues[j].q.empty()){
                current = j;
                break;
            }
        }
        if(queues[current].id == 5 && procRunning == true){
            queues[current].q.front().burst_time--;
        }

        if(procRunning == true && processCounter < queues[current].time_quantum && queues[current].id != 5){
            queues[current].q.front().burst_time--;
            processCounter++;
        }
        
        if(!queues[current].q.empty()){
            if(procRunning != true){
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                processCounter = 0;
                procRunning = true;
            }
        }

        if(queues[current].q.front().burst_time == 0 && !queues[current].q.empty()) {
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Finished @\t" + to_string(clock) +"\n";
                procAmount--;
                queues[current].q.pop();
                procRunning = false;
        } 
        if(processCounter == queues[current].time_quantum && !queues[current].q.empty() && queues[current].id != 5){
            cout << "Process " + to_string(queues[current].q.front().pid) + ": Switched @\t" + to_string(clock) +"\n";
            queues[current+1].q.push(queues[current].q.front()); 
            if(!queues[current].q.empty()){
                queues[current].q.pop();
            }
            procRunning = false;
        }
        clock++;
    }
    



    
    return 0;



}