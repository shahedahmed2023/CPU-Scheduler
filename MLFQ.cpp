#include <iostream>
#include <deque>
#include <vector>
#include <string>
#include <iomanip>
#include <queue>

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
    int tqsave = 0;
    int counter = 0;
    int Queue = 1;
};

// Queues for High Priority, Low Priority, and FCFS (store pointers)
deque<Process*> highPriority;
deque<Process*> q2;
deque<Process*> fcfs;
vector<Process*> deviceQueue; // Vector for I/O processes (store pointers)
float totaltime = 0; // Total time elapsed
float totalallcpu = 0; // Total CPU time utilized

// print result
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


    cout << setw(8) << "Wait Times" << setw(20) << "Turnaround Times" << setw(20) << "Response Times" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << " p1| " << setw(7) << p1.wait << setw(17) << p1.turnaroundtime << setw(20) << p1.responsetime << endl;
    cout << " p2| " << setw(7) << p2.wait << setw(17) << p2.turnaroundtime << setw(20) << p2.responsetime << endl;
    cout << " p3| " << setw(7) << p3.wait << setw(17) << p3.turnaroundtime << setw(20) << p3.responsetime << endl;
    cout << " p4| " << setw(7) << p4.wait << setw(17) << p4.turnaroundtime << setw(20) << p4.responsetime << endl;
    cout << " p5| " << setw(7) << p5.wait << setw(17) << p5.turnaroundtime << setw(20) << p5.responsetime << endl;
    cout << " p6| " << setw(7) << p6.wait << setw(17) << p6.turnaroundtime << setw(20) << p6.responsetime << endl;
    cout << " p7| " << setw(7) << p7.wait << setw(17) << p7.turnaroundtime << setw(20) << p7.responsetime << endl;
    cout << " p8| " << setw(7) << p8.wait << setw(17) << p8.turnaroundtime << setw(20) << p8.responsetime << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "Avg| " << setw(7) << waitaveg << setw(17) << turnaroundave << setw(20) << responseave << endl;


}
// print excutaion
void printexecution(Process* p) {
    cout << "---------------------------------------------------" << endl;
    cout << "Current Time: " << totaltime << " | Process " << p->name << " is Running" << endl;

    // Print processes in the high-priority queue
    cout << "\nHigh Priority Queue:\n";
    if (highPriority.empty()) {
        cout << "None\n";
    } else {
        for (Process* proc : highPriority) {
            cout << proc->name << " (CPU Burst: " << proc->data.front() << ")\n";
        }
    }

    // Print processes in the low-priority queue
    cout << "\nLow Priority Queue:\n";
    if (q2.empty()) {
        cout << "None\n";
    } else {
        for (Process* proc : q2) {
            cout << proc->name << " (CPU Burst: " << proc->data.front() << ")\n";
        }
    }

    // Print processes in the FCFS queue
    cout << "\nFCFS Queue:\n";
    if (fcfs.empty()) {
        cout << "None\n";
    } else {
        for (Process* proc : fcfs) {
            cout << proc->name << " (CPU Burst: " << proc->data.front() << ")\n";
        }
    }

    // Print processes currently in the I/O device queue
    cout << "\nProcesses in I/O:\n";
    if (deviceQueue.empty()) {
        cout << "None\n";
    } else {
        for (Process* proc : deviceQueue) {
            int remaining_io_time = proc->ioburst - proc->iocounter;
            cout << proc->name << " (Remaining I/O Time: " << remaining_io_time << ")\n";
        }
    }

    cout << "---------------------------------------------------" << endl;
}


int main() {
    int Tq = 5; // Time quantum for high-priority processes
    int Tq2 = 10; // Time quantum for low-priority processes

    // Queue to transfer processes from one queue to another
    queue<Process*> collect;

    // Initialize processes with their respective burst times
    Process p1("P1", {5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4});
    Process p2("P2", {4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8});
    Process p3("P3", {8, 33, 12, 41, 18, 65, 14, 21, 4, 61, 15, 18, 14, 26, 5, 31, 6});
    Process p4("P4", {3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3});
    Process p5("P5", {16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4});
    Process p6("P6", {11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8});
    Process p7("P7", {14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10});
    Process p8("P8", {4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6});

    // Populate the high priority queue with the processes
    highPriority.push_back(&p1);
    highPriority.push_back(&p2);
    highPriority.push_back(&p3);
    highPriority.push_back(&p4);
    highPriority.push_back(&p5);
    highPriority.push_back(&p6);
    highPriority.push_back(&p7);
    highPriority.push_back(&p8);

    int cpuburst = 0; // Variable to track the current CPU burst time
    int numprocesscomplete = 0; // Counter for completed processes

    // Main scheduling loop
    while (numprocesscomplete != 8) { // Continue until all processes are complete

        if (!highPriority.empty()) {
            Process* p = highPriority.front(); // Get the next process from high priority

            // Execute CPU burst
            if (p->data[0] > Tq) { // If the burst time is greater than the time quantum
                p->data[0] -= Tq; // Reduce the burst time
                cpuburst = Tq; // Set the CPU burst time
                p->Queue = 2;
                collect.push(highPriority.front()); // Move process to collect queue
                highPriority.pop_front(); // Remove process from high priority

            }
            else { // If the burst time is less than or equal to the time quantum
                cpuburst = p->data[0]; // Set CPU burst time to remaining burst
                p->data.erase(p->data.begin()); // Remove the burst time
                highPriority.pop_front(); // Remove process from high priority

            }

            // First run logic to capture response time
            if (p->firstrun) {
                p->responsetime = p->wait;  // Capture the response time when first run
                p->firstrun = false; // Mark as first run completed
            }

            p->totalcpu += cpuburst; // Accumulate total CPU time

            // Print execution details for the current process
            printexecution(p);

            // Simulate CPU execution for the duration of the burst
            for (int i = 0; i < cpuburst; i++) {
                totalallcpu++; // Increment total CPU time
                totaltime++; // Increment total time

                // Update waiting times for processes in highPriority
                for (int j = 0; j < highPriority.size(); j++) {
                   highPriority[j]->wait++; // Increment wait time for each process
                }

                // Update waiting times for processes in q2
                for (int j = 0; j < q2.size(); j++) {
                  q2[j]->wait++; // Increment wait time for each process
                }

                // Update waiting times for processes in FCFS queue
                for (int j = 0; j < fcfs.size(); j++) {
                    fcfs[j]->wait++; // Increment wait time for each process
                }

                // Check for processes in I/O and increment their counters
                for (int k = 0; k < deviceQueue.size(); k++) {
                    deviceQueue[k]->iocounter++; // Increment I/O counter

                    // If I/O is complete, move back to the ready queue
                    if (deviceQueue[k]->iocounter >= deviceQueue[k]->ioburst) {
                        deviceQueue[k]->ioburst = 0;
                        deviceQueue[k]->iocounter = 0; // Reset I/O counter

                        if (deviceQueue[k]->Queue == 1)
                            highPriority.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 2)
                            q2.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 3)
                            fcfs.push_back(deviceQueue[k]); // Move to ready queue
                        deviceQueue.erase(deviceQueue.begin() + k); // Remove from I/O
                        k--; // Adjust index due to removal
                    }
                }
            }

            // If there are processes to move from collect to q2
            if (!collect.empty()) {
                q2.push_back(collect.front()); // Move from collect to q2
                collect.pop(); // Pop the front of collect
            } else {
                // Handle I/O burst for current process
                if (!p->data.empty()) {
                    p->ioburst = p->data[0]; // Set the I/O burst
                    p->totalio += p->ioburst; // Accumulate I/O time
                    deviceQueue.push_back(p); // Move to I/O
                    p->data.erase(p->data.begin()); // Remove the used burst time

                    cout << "\nCompleted Execution= NO" << endl;
                } else {
                    p->turnaroundtime = p->totalcpu + p->totalio + p->wait; // Calculate turnaround time
                    numprocesscomplete++; // Increment completed process count
                    cout << "\nCompleted Execution= YES" << endl;
                }
                cout << "\n---------------------------------------------------\n\n\n";
            }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        } else if (highPriority.empty() && !q2.empty()) {
            Process* p = q2.front(); // Get the next process from low priority
            q2.pop_front(); // Remove from low priority
            // Execute CPU burst
            if (p->data[0] + p->counter > Tq2) { // Check if the remaining burst time exceeds time quantum
                p->Queue = 3;

                if (p->tqsave != 0) {

                    p->data[0] -= p->tqsave; // Use the saved time quantum
                    cpuburst = p->tqsave; // Set the CPU burst time
                    p->tqsave = 0; // Reset saved time quantum
                    p->counter = 0; // Reset counter
                    collect.push(p); // Move process to collect queue
                }
                else {
                    p->data[0] -= Tq2; // Use the full time quantum
                    cpuburst = Tq2; // Set the CPU burst time
                    p->totalcpu += cpuburst; // Accumulate total CPU time
                    collect.push(p); // Move process to collect queue

                }
            }
            else { // If the remaining burst time is less than or equal to the time quantum
                p->Queue = 2;

                if (p->tqsave != 0) {
                    cpuburst = p->tqsave; // Set CPU burst time to saved time quantum
                    p->data.erase(p->data.begin()); // Remove the burst time
                    p->tqsave = 0; // Reset saved time quantum
                }
                else {

                    cpuburst = p->data[0]; // Set CPU burst time to remaining burst time
                    p->data.erase(p->data.begin()); // Remove the burst time

                }
            }

            // Print execution details for the current low-priority process
            printexecution(p);
            // Execute CPU burst
            for (int i = 0; i < cpuburst; i++) {
                p->counter++; // Increment counter for CPU burst
                totalallcpu++; // Increment total CPU time
                totaltime++; // Increment total time

                // Update waiting times for processes in q2
                for (int j = 0; j < q2.size(); j++) {
                   q2[j]->wait++; // Increment wait time for each process
                }

                // Update waiting times for processes in FCFS queue
                for (int j = 0; j < fcfs.size(); j++) {
                    fcfs[j]->wait+=1; // Increment wait time for each process
                }

                // Check for processes in I/O and increment their counters
                for (int k = 0; k < deviceQueue.size(); k++) {
                    deviceQueue[k]->iocounter++; // Increment I/O counter

                    // If I/O is complete, move back to the ready queue
                    if (deviceQueue[k]->iocounter >= deviceQueue[k]->ioburst) {
                        deviceQueue[k]->ioburst = 0;
                        deviceQueue[k]->iocounter = 0; // Reset I/O counter
                        if (deviceQueue[k]->Queue == 1)
                            highPriority.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 2)
                            q2.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 3)
                            fcfs.push_back(deviceQueue[k]); // Move to ready queue
                        deviceQueue.erase(deviceQueue.begin() + k); // Remove from I/O
                        k--; // Adjust index due to removal
                    }
                }

                // Break if there are processes in high priority queue
                if (!highPriority.empty())
                    break;
            }

            // If highPriority is not empty, move process from collect to low priority
            if (!highPriority.empty() ) {
                if (!collect.empty()) {
                    // Increment the burst time of the front process by 'count' before pushing
                    collect.front()->data[0] += cpuburst - p->counter; // Adjust the burst time
                    collect.front()->tqsave = cpuburst - p->counter; // Save the remaining burst time
                    q2.push_back(collect.front()); // Push the process to low priority queue
                    collect.pop(); // Remove it from the collect queue
                    continue; // Continue to the next iteration of the loop
                } else {
                    p->data.insert(p->data.begin(), cpuburst - p->counter); // Insert remaining burst time
                    p->tqsave = cpuburst - p->counter; // Save the remaining burst time
                    p->counter = 0; // Reset counter
                    q2.push_back(p); // Push to low priority queue
                    continue; // Continue to the next iteration of the loop
                }
            } else {
                p->counter = 0; // Reset counter if no high priority processes
            }

            // If there are processes to move from collect to FCFS
            if (!collect.empty()) {
                fcfs.push_back(collect.front()); // Move from collect to fcfs
                collect.pop(); // Pop the front of collect
            }
            else {
                // Handle I/O burst for the current process
                if (!p->data.empty()) {
                    p->ioburst = p->data[0]; // Set the I/O burst
                    p->totalio += p->ioburst; // Accumulate I/O time
                    deviceQueue.push_back(p); // Move to I/O
                    p->data.erase(p->data.begin()); // Remove the used burst time

                    cout << "\nCompleted Execution= NO" << endl;
                } else {
                    p->turnaroundtime = p->totalcpu + p->totalio + p->wait; // Calculate turnaround time
                    numprocesscomplete++; // Increment completed process count
                    cout << "\nCompleted Execution= YES" << endl;
                }
                cout << "\n---------------------------------------------------\n\n\n";
            }
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        } else if (highPriority.empty() && q2.empty() && !fcfs.empty()) {
            Process* p = fcfs.front(); // Get the next process from FCFS
            fcfs.pop_front();

            if (p->tqsave != 0) { // If there's a saved time quantum
                cpuburst = p->tqsave; // Set CPU burst time to saved time quantum
                p->data.erase(p->data.begin()); // Remove the burst time
                p->tqsave = 0; // Reset saved time quantum


            } else {
                cpuburst = p->data[0]; // Get the CPU burst time
                p->totalcpu += cpuburst; // Accumulate total CPU time
                p->data.erase(p->data.begin()); // Remove the used burst time

            }

            // Print execution details for the current process
            printexecution(p);
            for (int i = 0; i < cpuburst; i++) {
                totalallcpu++; // Increment total CPU time
                totaltime++; // Increment total time
                p->counter++; // Increment counter

                // Update waiting times for processes in FCFS queue
                for (int j = 0; j < fcfs.size(); j++) {
                    fcfs[j]->wait +=1 ;
                }

                // Check for processes in I/O and increment their counters
                for (int k = 0; k < deviceQueue.size(); k++) {
                    deviceQueue[k]->iocounter++; // Increment I/O counter

                    // If I/O is complete, move back to the ready queue
                    if (deviceQueue[k]->iocounter >= deviceQueue[k]->ioburst) {
                        deviceQueue[k]->ioburst = 0;
                        deviceQueue[k]->iocounter = 0; // Reset I/O counter
                       if (deviceQueue[k]->Queue == 1)
                            highPriority.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 2)
                            q2.push_back(deviceQueue[k]); // Move to ready queue
                        if (deviceQueue[k]->Queue == 3)
                            fcfs.push_back(deviceQueue[k]); // Move to ready queue
                        deviceQueue.erase(deviceQueue.begin() + k); // Remove from I/O
                        k--; // Adjust index due to removal
                    }
                }
                // Break if there are processes in high priority queue
                if (!highPriority.empty() || !q2.empty())
                    break;
            }

            if (!highPriority.empty()|| !q2.empty()) {
                p->data.insert(p->data.begin(), cpuburst - p->counter); // Insert remaining burst time
                p->tqsave = cpuburst - p->counter; // Save remaining burst time
                p->counter = 0; // Reset counter
                fcfs.push_back(p); // Push to fcfs queue
                continue; // Continue to next iteration
            }
            else {
                p->counter = 0; // Reset counter if no high priority processes
            }

            // Handle I/O burst for the current process

            if (!p->data.empty()) {
                p->ioburst = p->data[0]; // Set the I/O burst
                p->totalio += p->ioburst; // Accumulate I/O time
                deviceQueue.push_back(p); // Move to I/O
                p->data.erase(p->data.begin()); // Remove the used burst time

                cout << "\nCompleted Execution= NO" << endl;
            } else {
                p->turnaroundtime = p->totalcpu + p->totalio + p->wait; // Calculate turnaround time
                numprocesscomplete++; // Increment completed process count
                cout << "\nCompleted Execution= YES" << endl;
            }
            cout << "\n---------------------------------------------------\n\n\n";
        }

        else if (q2.empty() && fcfs.empty() && highPriority.empty() && !deviceQueue.empty()) {
            // Handles CPU idle incrementing Processes in I/O burst counters
            totaltime++; // Increment total time once per iteration
            // Iterate through the device queue and update I/O counters
            for (int n = 0; n < deviceQueue.size(); n++) {
                deviceQueue[n]->iocounter++; // Increment I/O counter for each process

                // Check if the I/O is complete
                if (deviceQueue[n]->iocounter >= deviceQueue[n]->ioburst) {
                    // Move the completed process back to the ready queue
                    deviceQueue[n]->ioburst = 0;
                    deviceQueue[n]->iocounter = 0;
                    if (deviceQueue[n]->Queue == 1)
                            highPriority.push_back(deviceQueue[n]); // Move to ready queue
                    if (deviceQueue[n]->Queue == 2)
                            q2.push_back(deviceQueue[n]); // Move to ready queue
                    if (deviceQueue[n]->Queue == 3)
                            fcfs.push_back(deviceQueue[n]); // Move to ready queue
                    deviceQueue.erase(deviceQueue.begin() + n); // Remove from device queue
                    n--; // Adjust index because we removed the current element
                }
            }
        }
    }

    printresults(p1, p2, p3, p4, p5, p6, p7, p8); // Print final results for all processes
    return 0; // Return success status
}
