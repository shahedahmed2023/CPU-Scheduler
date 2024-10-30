#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Process {
    Process(string n, vector<int> d) : name(n), data(d) {}

    string name; // Process name
    vector<int> data; // Burst times
    int wait = 0; // Wait time
    int ioburst = 0; // I/O burst time
    int iocounter = 0; // Time spent in I/O

    bool firstrun = true; // Flag to check if it’s the first run
    int responsetime = 0; // Response time

    int turnaroundtime = 0; // Turnaround time
    int totalcpu = 0; // Total CPU time
    int totalio = 0; // Total I/O time
};

queue<Process*> readyqueue; // Queue for ready processes (store pointers)
vector<Process*> deviceQueue; // Vector for I/O processes (store pointers)
float totaltime = 0; // Total time elapsed
float totalallcpu = 0; // Total CPU time utilized

void printexecution(Process* p) {
    cout << "---------------------------------------------------" << endl;
    cout << "Execution Time is " << totaltime << "            " << p->name << " is Running" << endl;

    // Print the processes in the ready queue
    cout << "Processes in Ready Queue:\n";
    queue<Process*> tempQueue1 = readyqueue; // Temporary queue to preserve original
    while (!tempQueue1.empty()) {
        cout << tempQueue1.front()->name << " CPU Burst Length " << tempQueue1.front()->data.front() << "\n";
        tempQueue1.pop();
    }

    // Print processes in I/O
    cout << "Processes in I/O:\n";
    vector<Process*> tempQueue2 = deviceQueue; // Make sure to create a temporary queue for deviceQueue

    for(int i = 0; i < tempQueue2.size();i++) {
        int timeremaining = tempQueue2[i]->ioburst - tempQueue2[i]->iocounter;
        cout << tempQueue2[i]->name << " Time Remaining in I/O: " << timeremaining << "\n";
    }
}


void printresults(const Process p1,const Process p2,const Process p3,const Process p4,const Process p5,const Process p6,const Process p7,const Process p8) {
    int totalwait = p1.wait + p2.wait + p3.wait + p4.wait + p5.wait + p6.wait + p7.wait + p8.wait;
    double waitaveg = totalwait / 8;
    float totalturnaroundtime = p1.turnaroundtime + p2.turnaroundtime + p3.turnaroundtime + p4.turnaroundtime + p5.turnaroundtime + p6.turnaroundtime + p7.turnaroundtime + p8.turnaroundtime;
    float turnaroundave = totalturnaroundtime / 8;
    float totalresponsetime = p1.responsetime + p2.responsetime + p3.responsetime + p4.responsetime + p5.responsetime + p6.responsetime + p7.responsetime + p8.responsetime;
    float responseave = totalresponsetime / 8;
    cout << "Total Time = " << totaltime << endl;
    float cpuutilization = (totalallcpu / totaltime) * 100;
    cout << "CPU Utilization = " << cpuutilization << "%" << endl;


    cout  <<setw(15)<< "Wait Times" << setw(20) << "Turnaround Times" << setw(20) << "Response Times" << endl;
    cout << "-------------------------------------------------------" << endl;;
    cout << " p1| " << p1.wait <<setw(20)<< p1.turnaroundtime <<setw(20)<< p1.responsetime << endl;
    cout << " p2| " << p2.wait <<setw(20)<< p2.turnaroundtime<<setw(20)<< p2.responsetime << endl;
    cout << " p3| " << p3.wait <<setw(20)<< p3.turnaroundtime<<setw(20)<< p3.responsetime << endl;
    cout << " p4| " << p4.wait <<setw(20)<< p4.turnaroundtime<<setw(20)<< p4.responsetime << endl;
    cout << " p5| " << p5.wait <<setw(20)<< p5.turnaroundtime<< setw(20)<< p5.responsetime << endl;
    cout << " p6| " << p6.wait <<setw(20)<< p6.turnaroundtime<< setw(20)<< p6.responsetime << endl;
    cout << " p7| " << p7.wait <<setw(20)<< p7.turnaroundtime<< setw(20)<< p7.responsetime << endl;
    cout << " p8| " << p8.wait <<setw(20)<< p8.turnaroundtime<< setw(20)<< p8.responsetime << endl;
    cout << "-------------------------------------------------------" << endl;;
    cout << "Avg| "<< waitaveg<<setw(21)<< turnaroundave<<setw(21) << responseave << endl;

}

int main() {
    Process p1("P1", {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4});
    Process p2("P2", {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8});
    Process p3("P3", {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6});
    Process p4("P4", {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3});
    Process p5("P5", {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4});
    Process p6("P6", {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8});
    Process p7("P7", {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10});
    Process p8("P8", {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6});

    readyqueue.push(&p1);
    readyqueue.push(&p2);
    readyqueue.push(&p3);
    readyqueue.push(&p4);
    readyqueue.push(&p5);
    readyqueue.push(&p6);
    readyqueue.push(&p7);
    readyqueue.push(&p8);

    int cpuburst = 0;
    int numprocesscomplete = 0;

    // Main scheduling loop
    while (numprocesscomplete != 8) {
        if (!readyqueue.empty()) {
            Process* p = readyqueue.front(); // Get the next process
            readyqueue.pop();

            // First run logic
            if (p->firstrun) {
                p->responsetime = p->wait;  // Capture the response time when first run
                p->firstrun = false;
            }

            cpuburst = p->data[0]; // Get the CPU burst time
            p->totalcpu += cpuburst; // Accumulate total CPU time
            p->data.erase(p->data.begin()); // Remove the used burst time

            printexecution(p); // Print the execution details

            // Execute CPU burst
            for (int i = 0; i < cpuburst; i++) {
                totalallcpu++; // Increment total CPU time
                totaltime++; // Increment total time

                for (int j = 0; j < readyqueue.size(); j++) {
                    Process* temp = readyqueue.front();
                    readyqueue.pop();
                    temp->wait++; // Increment wait time
                    readyqueue.push(temp); // Push back
                }

                // Check for processes in I/O
                for (int k = 0; k < deviceQueue.size(); k++) {
                    deviceQueue[k]->iocounter++; // Increment I/O counter

                    // If I/O is complete, move back to the ready queue
                    if (deviceQueue[k]->iocounter == deviceQueue[k]->ioburst) {
                        deviceQueue[k]->ioburst = 0;
                        deviceQueue[k]->iocounter = 0; // Reset I/O counter
                        readyqueue.push(deviceQueue[k]); // Push to ready queue
                        deviceQueue.erase(deviceQueue.begin() + k); // Remove from I/O
                        k--; // Adjust index due to removal
                    }
                }

            }

            // Handle I/O burst
            if (!p->data.empty()) {
                p->ioburst = p->data[0]; // Set the I/O burst
                p->totalio += p->ioburst; // Accumulate I/O time
                p->data.erase(p->data.begin()); // Remove the used burst time
                deviceQueue.push_back(p); // Move to I/O
                cout << "\nCompleted Execution= NO" << endl;
            } else {
                p->turnaroundtime = p->totalcpu + p->totalio + p->wait; // Calculate turnaround time
                numprocesscomplete++; // Increment completed process count
                cout << "\nCompleted Execution= YES" << endl;
            }
            cout << "\n---------------------------------------------------\n\n\n";
        }

    else if (readyqueue.empty() && !deviceQueue.empty()) { // Handles CPU Idle incrementing Processes in I/O Burst counters
            totaltime++; // Increment total time once per iteration
            // Iterate through the device queue and update I/O counters
            for (int n = 0; n < deviceQueue.size(); n++) {
                deviceQueue[n]->iocounter++; // Increment I/O counter for each process

                // Check if the I/O is complete
                if (deviceQueue[n]->iocounter >= deviceQueue[n]->ioburst) {
                    // Move the completed process back to the ready queue
                    deviceQueue[n]->ioburst = 0;
                    deviceQueue[n]->iocounter = 0;
                    readyqueue.push(deviceQueue[n]);
                    deviceQueue.erase(deviceQueue.begin() + n); // Remove from device queue
                    n--; // Adjust index because we removed the current element
                }
            }
        }

    }
    printresults(p1, p2, p3, p4, p5, p6, p7, p8); // Print final results
    return 0;
}
