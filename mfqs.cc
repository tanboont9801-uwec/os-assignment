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
    string id;
    int time_quantum;
    queue<process> q;
};

void sort_arrival(vector<process>& v){
    sort(v.begin(), v.end(), [](process a, process b){
        return a.arrival_time < b.arrival_time;
    });
}
/**
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
*/

void queue_processing(vector<process>& v, vector<que>& q){  

    //**** TO DO: assign new processes into first queue. Not done, push process to q.next()
   for(int index = 0; index < v.size(); index++){
    for(int j = 0; j < q.size(); j++)
            if(v[index].burst_time > 0){
                q.push_back(v.at(index));
            }
            
        
   }

   //*** 
   // queue.next() is go to the next queue 
   

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
    if(file.is_open()) {
        getline(file, nothing);
        int pid, burst, arrival;

        while(file >> pid >> burst >> arrival){
            // ***** 
            if(burst > 0 || arrival > 0){  
            processes.push_back(process());
            processes[counter].pid = pid;
            processes[counter].burst_time = burst;
            processes[counter].arrival_time = arrival;
            counter++;
            }
        }
        file.close();   
    }else{
        cout<< "Unable to open file\n";
    }

    //call function to sort the vector of processes by the arrival time
    sort_arrival(processes);
    //filter(processes);


    //intializes queues with the time quantum based on the parameters
    int count = 0;
    vector<que> queues;
    while(count < num_queues-1){
        string qid = "Q" + to_string((count+1));
        queues.push_back(que());
        queues[count].time_quantum = (time_q * pow(2, count));
        queues[count].id = qid;
        count++;
    }
    //initializes first come first serve queue
    queue<process> fcfs;
    int remain_bursttime = processes[count].burst_time;
    while(remain_bursttime > 0){
        remain_bursttime++;
    }
        fcfs.pop();


    int clock = 0;
    while(clock <= processes.size()){

    }

    
    return 0;



}
