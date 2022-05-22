#include <iostream>
#include <vector>  
#include <iomanip>

struct Process
{
    int id = 0;
    int arrival_time = 0;
    int burst_time = 0;
};

using namespace std;

// Ho tro swap 2 process
void swap(Process& p1, Process& p2)
{
    Process temp = p1;
    p1 = p2;
    p2 = temp;
}

// Nhap Process
void Nhap_Process(vector<Process>& Process_array, int soluongProcess)
{
    for (int i = 0; i < soluongProcess; i++)
    {
        int id, arrival_time, burst_time;
        cout << "Nhap Process ID, Arrival Time, Burst Time: ";
        cin >> id >> arrival_time >> burst_time;
        Process_array[i].id = id; // Process ID
        Process_array[i].arrival_time = arrival_time; //Arrival Time
        Process_array[i].burst_time = burst_time; // Burst Time
    }
}

// Xuat Process
void Xuat_Process(vector<Process> Process_array, int soluongProcess, vector<int> start_time, vector<int> finish_time)
{
    int turn_around_time = 0;
    int waiting_time = 0;
    int response_time = 0;
    cout << "\nID\tAT\tBT\tST\tFT\tTAT\tWT\tRP\n";
    for (int i = 0; i < soluongProcess; i++)
    {
        turn_around_time += finish_time[i] - Process_array[i].arrival_time;
        waiting_time += finish_time[i] - Process_array[i].arrival_time - Process_array[i].burst_time;
        response_time += start_time[i] - Process_array[i].arrival_time;
        cout << Process_array[i].id << "\t" // Process ID
            << Process_array[i].arrival_time << "\t" // Arrival Time
            << Process_array[i].burst_time << "\t" // Burst Time 
            << start_time[i] << "\t" // Start Time
            << finish_time[i] << "\t" // Finish Time
            << finish_time[i] - Process_array[i].arrival_time << "\t" // Turn Around Time
            << finish_time[i] - Process_array[i].arrival_time - Process_array[i].burst_time << "\t" // Waiting Time
            << start_time[i] - Process_array[i].arrival_time << "\t" // Response Time
            << endl;
    }

    cout << "\n>> Average Turn Around Time: " << fixed << setprecision(2) << turn_around_time / (double)soluongProcess << endl;
    cout << ">> Average Waiting Time: " << fixed << setprecision(2) << waiting_time / (double)soluongProcess << endl;
    cout << ">> Average Response Time: " << fixed << setprecision(2) << response_time / (double)soluongProcess  << endl;
}

// Bubble Sort theo thu tu arrival time
void Sap_xep_Process_theo_Arr(vector<Process>& Process_array, int soluongProcess)
{
    for (int i = 0; i < soluongProcess - 1; i++)
        for (int j = 0; j < soluongProcess - i - 1; j++)
            if (Process_array[j].arrival_time > Process_array[j + 1].arrival_time)
            {
                swap(Process_array[j], Process_array[j + 1]);
            }
}

// Dinh thoi Round Robin
void Scheduling(vector<Process> Process_array, int soluongProcess, int time_quantumn)
{
    // Khoi tao start time, finish time, flag_in_process
    // start_time: thoi gian bat dau
    // finish_time: thoi gian ket thuc
    // flag_in_process: cho biet Process da vao Ready queue chua?
    vector<Process> Process_temp = Process_array;
    vector<int> start_time;
    vector<int> finish_time;
    vector<int> flag_in_process;
    for (int i = 0; i < soluongProcess; i++)
    {
        start_time.push_back(0);
        finish_time.push_back(0);
        flag_in_process.push_back(0);
    }

    int current_time = 0;   // Bien thoi gian
    int completed_all_process = 0; // So Process da hoan thanh
    vector<Process> ReadyQueue; // Ready Queue

    // Track process[0] de biet khi nao Process 1 vao Ready queue
    Process p1 = Process_array[0];

    while (completed_all_process != soluongProcess)
    {
        if (current_time >= p1.arrival_time)    // Dieu kien de cho biet khi nao Process P1 bat dau vao ReadyQueue
        {
            if ((Process_array.empty() == false) 
                && (current_time >= Process_array[0].arrival_time))    // Dieu kien cho Process vao Ready Queue
            {
                ReadyQueue.push_back(Process_array.front());
                Process_array.erase(Process_array.begin());
            }
            if (ReadyQueue.empty() == false)
            {
                // Kiem tra Process chua tung vao ready queue
                int idx = ReadyQueue[0].id - 1;
                if (flag_in_process[idx] == 0)
                {
                    start_time[idx] = current_time;
                    flag_in_process[idx] = 1;
                }

                
                // Xu li Burst Time
                if (ReadyQueue[0].burst_time - time_quantumn < 0)
                {
                    current_time += ReadyQueue[0].burst_time;
                    ReadyQueue[0].burst_time = 0;
                }
                else
                {
                    ReadyQueue[0].burst_time -= time_quantumn;
                    current_time += time_quantumn;
                }

                // Xu li Finish Time
                if (ReadyQueue[0].burst_time <= 0)
                {
                    finish_time[idx] = current_time;
                    completed_all_process += 1;
                }

                // Track ReadyQueue[0]
                Process temp_process;
                if (ReadyQueue[0].burst_time > 0)
                {
                    temp_process = ReadyQueue[0];
                }

                // Xoa ReadyQueue[0] 
                ReadyQueue.erase(ReadyQueue.begin());

                // Kiem tra Process nao yeu cau CPU khi Process truoc dang thuc thi
                int tick = 0;
                vector<Process>temp = Process_array;
                for (int j = 0; j < temp.size(); j++)
                {
                    if (temp[j].arrival_time <= current_time)
                    {
                        ReadyQueue.push_back(temp[j]);
                        Process_array.erase(Process_array.begin());
                    }

                }

                // ReadyQueue duoc track thoa dieu kien duoc phep quay tro lai ReadyQueue
                if (temp_process.burst_time > 0)
                {
                    ReadyQueue.push_back(temp_process);
                }
            }
            else
            {
                // Neu khong co Process nao trong ReadyQueue thi them Process ke tiep
                Process p2 = Process_array.front();
                ReadyQueue.push_back(p2);
                current_time = p2.arrival_time;
                Process_array.erase(Process_array.begin());

            }
        }
        else
        {
            current_time += 1;
        }
    }

    // Xuat Process
    Xuat_Process(Process_temp, soluongProcess, start_time, finish_time);
}

int main()
{
    int soluongProcess; // So luong Process
    int time_quantumn;  // Quantumn Time

    cout << "Nhap so luong process: ";
    cin >> soluongProcess;

    cout << "Nhap Quantumn time: ";
    cin >> time_quantumn;

    vector<Process> Process_arr(soluongProcess);

    Nhap_Process(Process_arr, soluongProcess);

    Sap_xep_Process_theo_Arr(Process_arr, soluongProcess);

    Scheduling(Process_arr, soluongProcess, time_quantumn);

    cout << "\n---DONE---" << endl;
    return 0;
}