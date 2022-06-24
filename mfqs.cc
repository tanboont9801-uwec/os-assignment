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
    cout << "Running MFQS....... \n";
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
        int pid, burst, arrival, priority, ageindex;
        while(file >> pid >> burst >> arrival >> priority >> ageindex){
            processes.push_back(process());
            processes[counter].pid = pid;
            processes[counter].burst_time = burst;
            processes[counter].arrival_time = arrival;
            counter++;

            cout<<"pid: "<<endl;
            cout<<pid<<endl;

            cout<<"burst: "<<endl;
            cout<<burst<<endl;

            cout<<"arrival time: "<<endl;
            cout<<arrival<<endl;
        }
        file.close();   
    }else{
        cout<< "Unable to open file\n";
    }
    //exit(0);

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
    
    //*** TODO: calculate ave. wait time & ave turnaround time
/*
- sumCompletionTime = sumCompletionTime +  pid[current]( clock from "Runs @" - clock from "Finished @" );
    
*/

    int clock = 0;
    
    bool procRunning;
    int processCounter = 0;
    int current = 0;

    int sumCompletionTime = 0;
    int sumArrivalTime = 0;
    int sumBurstTime = 0;
    int start;
    // procAmount represents number of processes 
    int procAmount = processes.size();
        
    while(procAmount > 0) {
        for(int i = 0; i < processes.size(); i++){
            //cout<<" line 153: "<<processes[i].arrival_time<<endl;
            if(clock == processes[i].arrival_time){
                //cout<<"line 154"<<endl;
                queues.front().q.push(processes[i]);
                cout << "Process " + to_string(processes[i].pid) + ": Arrives @\t" + to_string(clock) +"\n";
                number_of_processes = number_of_processes + 1;
                sumArrivalTime++;
                //cout<<"line 160"<<endl;
            }
        }
        for(int j = 0; j<queues.size(); j++){
            if(!queues[j].q.empty()){
                current = j;
                //cout<<"line 166"<<endl;
                break;

                
            }   
                
                //cout<<"line 172"<<endl;
                continue;
                

        }
        if(queues[current].id == 5 && procRunning == true){
            // amount of remaining burst time decrease
            // sum of burst time increase
            queues[current].q.front().burst_time--;
            // not being printed
            //cout<<"line 182"<<endl;
               

        }

        if(procRunning == true && processCounter < queues[current].time_quantum && queues[current].id != 5){
            queues[current].q.front().burst_time--;
            processCounter++;
            //cout<<"line 190"<<endl;
                

        }
        
        /* modifies runs at */
        if(!queues[current].q.empty()){
            if(procRunning != true){
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Runs @\t" + to_string(clock) +"\n";
                processCounter = 0;
                procRunning = true;
                //cout<<"line 201"<<endl;
                

            }
            //cout<<"line 205"<<endl;
        }

        if(queues[current].q.front().burst_time == 0 && !queues[current].q.empty()) {
                cout << "Process " + to_string(queues[current].q.front().pid) + ": Finished @\t" + to_string(clock) +"\n";
                procAmount--;
                queues[current].q.pop();
                procRunning = false;
                sumCompletionTime++;
                //cout<<"line 214"<<endl;
                

        } 
        if(processCounter == queues[current].time_quantum && !queues[current].q.empty() && queues[current].id != 5){
            cout << "Process " + to_string(queues[current].q.front().pid) + ": Switched @\t" + to_string(clock) +"\n";
            queues[current+1].q.push(queues[current].q.front()); 
            if(!queues[current].q.empty()){
                queues[current].q.pop();
                
                // ?? not being printed
                //cout<<"line 225"<<endl;
                
            }
            // kiki and dug not being printed
            //cout<<"kiki"<<endl;
            procRunning = false;
            //cout<<"line 231"<<endl;
        }
        // if clock go over 20 >> exit
        //cout<<"line 234" <<clock<<endl;
        clock++;
        if(clock >= 20){
            break;
        }

        // infinite loop
        //cout<<"line 241";

        
    }
    
   
    /*
    int totalTurnaroundTime = sumCompletionTime - sumArrivalTime;
    int totalWaitingTime = totalTurnaroundTime - sumBurstTime;

    double aveTurnaroundTime;
    double aveWaitingTime; */




    
    return 0;



}
