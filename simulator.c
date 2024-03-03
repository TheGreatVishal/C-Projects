// OS PROJECT
// Simulator to compare different types of Scheduling Algorithms

// Interactive :
// 1. RR (Round Robin) [0]
// 2. Priority [1]
// 3. Lottery [2]

// Batch Scheduling :
// 4. FCFS [3]
// 5. SJF [4]
// 6. SRTN [5]
// 7. HRRN [6]

// Real-Time scheduling algorithm :
// 8. EDF [7]

// Comparisons (6) :
// 1. Throughput
// 2. CPU utilization
// 3. Context-switch
// 4. Average Waiting-Time
// 5. Average Turn-Around-Time
// 6. Response-Time

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int debug_RR = 0;       // to debug round robin
int debug_PRIORITY = 0; // to debug Priority
int debug_LOTTERY = 0;  // to debug Lottery
int debug_FCFS = 0;     // to debug FCFS
int debug_SJF = 0;      // to debug SJF
int debug_SRTN = 0;     // to debug SRTN
int debug_HRRN = 0;     // to debug HRRN
int debug_EDF = 1;      // to debug EDF

FILE *fptr_write;

int result_in_file = 0;     // to show result in file [1 : yes, 0 : no]
int result_on_terminal = 1; // to show result on terminal [1 : yes, 0 : no]

struct process
{
    int id;  // process id
    int at;  // arrival time
    int bt;  // burst time
    int rbt; // remaining bt
    int period;
    int current_deadline;
    int no_of_execution;
    int no_of_execution_buffer;

    int priority;   // for priority scheduling algorithm
    int tickets[2]; // to maintain tickets {0:lower-ticket, 1:upper-ticket}

    int ct;     // completion time
    double wt;  // waiting time
    double tat; // turn-around time
    double rt;  // response-time
};
typedef struct process process;

struct result
{
    double awt;         // average waiting-time
    double att;         // average turn-around time
    double art;         // average response time
    int context_switch; // context-switches
    double throughput;  // no. of processes completed per unit time
};
typedef struct result result;

void initialize_final_result(result *final_result);
int generate_random_number(int lower, int upper);
void generate_tickets(process *processes, int n);
void generateProcesses(process *ps, int n);
void set_RBT_RT(process *ps, int n);
void set_processes(process *ps, int n);
void set_Priority(process *ps, int n);
void set_current_deadline_AT(process *ps, int n);
void set_Execution_buffer(process *ps, int n);

void sort_by_arrival(process *ps, int n);
void sort_by_priority(process *ps, int n);
void sort_by_index(process *ps, int no_of_process);
void sort_by_deadline(process *ps, int n);

void round_robin(process *ps, int time_quantum, int no_of_process, result *final_result);
void priority_scheduling(process *ps, int no_of_process, result *final_result);
int get_process_index(process *ps, int no_of_process, int ticket_number);
void lottery_scheduling(process *ps, int time_quantum, int no_of_process, result *final_result);
void fcfs_scheduling(process *ps, int no_of_process, result *final_result);
void sjf_scheduling(process *ps, int no_of_process, result *final_result);
void srtn_scheduling(process *ps, int no_of_process, result *final_result);
void hrrn_scheduling(process *ps, int no_of_process, result *final_result);
void edf_scheduling(process *ps, int no_of_process, result *final_result);

void separate_results(result *final_result, int i);

void calculate_TAT_WT(process *ps, int no_of_process);
void calculate_AWT_ATT_ART(process *ps, int no_of_process, result *final_result, int index);

void display(process *ps, int n);  // displays complete details of generated processes
void display2(process *ps, int n); // displays result (S.NO, ID, AT, BT, TAT, WT, RT, CT)
void display_Basic_process_details(process *ps, int n);
void display_priority_process_details(process *ps, int n);
void display_Lottery_process_details(process *ps, int n);
void display_EDF_details(process *ps, int n);
void display_result(result *final_result); // display final comparison chart
void display_AWT(result *final_result, int n);
void display_ATT(result *final_result, int n);
void display_ART(result *final_result, int n);
void display_Context_switch(result *final_result, int n);
void display_throughput(result *final_result, int i);

void dotted_line();
void dotted_line_in_file();

void main()
{
    dotted_line();
    int choice;
    srand(time(NULL)); // for random no. generator
    int n, time_quantum = 2;
    result final_result[8]; // holds AWT and ATT of all processes

    initialize_final_result(final_result);
    // dotted_line();
    printf("\nWelcome to CPU Scheduling Simulator.");
    printf("\n\nScheduling Algorithms : ");
    printf("\n1. RR\n2. Priority\n3. Lottery\n4. FCFS\n5. SJF\n6. SRTN\n7. HRRN\n8. EDF\n9. ALL Together");
    printf("\n\nChoose Algorithm to be Simulated : ");
    scanf("%d", &choice);
    // printf("\n\nEnter Number of process : ");
    // scanf("%d", &n); // No. of processes
    n = 3;
    process ps[n]; // array which holds all processes

    set_processes(ps, n);
    printf("\nGenerated %d Processes Successfully", n);

    fptr_write = fopen("output.txt", "w");

    switch (choice)
    {
    case 1:
        // 0----------------------------Round-Robin---------------------------
        display_Basic_process_details(ps, n);
        round_robin(ps, time_quantum, n, final_result); // Time-quantum = 2, n = no. of processes
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of Round-Robin");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result
            printf("\n-> Histogram for Round-Robin\n");
            separate_results(final_result, 0);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of Round-Robin", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result
            fputs("\n-> Histogram for Round-Robin\n", fptr_write);
            separate_results(final_result, 0);
            dotted_line_in_file();
        }
        break;
    case 2:
        // 1-----------------------Priority-----------------------------------

        set_Priority(ps, n);
        display_priority_process_details(ps, n);
        priority_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of Priority-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result
            printf("\n-> Histogram for Priority\n");
            separate_results(final_result, 1);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of Priority-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Priority Result
            fputs("\n-> Histogram for Priority-Scheduling\n", fptr_write);
            separate_results(final_result, 1);
            dotted_line_in_file();
        }

        break;
    case 3:
        // 2-----------------------Lottery-----------------------------------
        generate_tickets(ps, n);
        display_Lottery_process_details(ps, n);
        lottery_scheduling(ps, time_quantum, n, final_result); // Time-quantum = 2, n = no. of processes
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of Lottery-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Lottery-Scheduling Result
            printf("\n-> Histogram for Lottery\n");
            separate_results(final_result, 2);
            dotted_line();
        }

        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of Lottery-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Lottery-Scheduling Result
            fputs("\n-> Histogram for Lottery-Scheduling\n", fptr_write);
            separate_results(final_result, 2);
            dotted_line_in_file();
        }

        break;
    case 4:
        // 3-----------------------FCFS-----------------------------------
        display_Basic_process_details(ps, n);
        fcfs_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of FCFS-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying FCFS-Scheduling Result
            printf("\n-> Histogram for FCFS\n");
            separate_results(final_result, 3);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of FCFS-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying FCFS-Scheduling Result
            fputs("\n-> Histogram for FCFS-Scheduling\n", fptr_write);
            separate_results(final_result, 3);
            dotted_line_in_file();
        }
        break;
    case 5:
        // 4-----------------------SJF-----------------------------------
        display_Basic_process_details(ps, n);
        sjf_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of SJF-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying SJF-Scheduling Result
            printf("\n-> Histogram for SJF\n");
            separate_results(final_result, 4);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of SJF-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying SJF-Scheduling Result
            fputs("\n-> Histogram for SJF-Scheduling\n", fptr_write);
            separate_results(final_result, 4);
            dotted_line_in_file();
        }
        break;
    case 6:
        // 5-----------------------SRTN-----------------------------------
        display_Basic_process_details(ps, n);
        srtn_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of SRTN-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying SRTN-Scheduling Result
            printf("\n-> Histogram for SRTN\n");
            separate_results(final_result, 5);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of SRTN-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying SRTN-Scheduling Result
            fputs("\n-> Histogram for SRTN-Scheduling\n", fptr_write);
            separate_results(final_result, 5);
            dotted_line_in_file();
        }
        break;
    case 7:
        // // 6-----------------------HRRN-----------------------------------
        display_Basic_process_details(ps, n);
        hrrn_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of HRRN-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying HRRN-Scheduling Result
            printf("\n-> Histogram for HRRN\n");
            separate_results(final_result, 6);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            // fptr_write = fopen("output.txt", "w");
            dotted_line_in_file();
            fputs("\n\n-> Result of HRRN-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying HRRN-Scheduling Result
            fputs("\n-> Histogram for HRRN-Scheduling\n", fptr_write);
            separate_results(final_result, 6);
            dotted_line_in_file();
        }
        break;
    case 8:
        // 7-----------------------EDF-----------------------------------
        set_current_deadline_AT(ps, n);
        display_EDF_details(ps, n);
        set_Execution_buffer(ps, n);
        edf_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            dotted_line();
            printf("\n\n-> Result of EDF-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying EDF-Scheduling Result

            printf("\nHistogram under development\n");
            printf("\n-> Histogram for EDF-Scheduling\n");
            separate_results(final_result, 7);
            dotted_line();
        }
        // putting result in file
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            dotted_line();
            dotted_line_in_file();
            fputs("\n\n-> Result of EDF-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying EDF-Scheduling Result
            // fputs("\n-> Histogram for EDF-Scheduling\n", fptr_write);
            // separate_results(final_result, 7);
            dotted_line_in_file();
        }

        // printf("\n\nEDF Under Development !!\n\n");
        break;

    case 9:
        set_Priority(ps, n);
        generate_tickets(ps, n);
        fptr_write = fopen("output.txt", "w");
        display(ps, n);

        // 0----------------------------Round-Robin---------------------------
        round_robin(ps, time_quantum, n, final_result); // Time-quantum = 2, n = no. of processes
        if (result_on_terminal)
        {
            printf("\n-> Result of Round-Robin");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result
        }
        if (result_in_file)
        {
            printf("\n\nPlease see output.txt for results !!\n");
            // fptr_write = fopen("output.txt", "a");
            dotted_line_in_file();
            fputs("\n\n-> Result of Round-Robin", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result

            dotted_line_in_file();
        }

        // 1-----------------------Priority-----------------------------------
        priority_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of Priority-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of Priority-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Round Robin Result

            dotted_line_in_file();
        }

        // 2-----------------------Lottery-----------------------------------
        lottery_scheduling(ps, time_quantum, n, final_result); // Time-quantum = 2, n = no. of processes
        if (result_on_terminal)
        {
            printf("\n-> Result of Lottery-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying Lottery-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of Lottery-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying Lottery-Scheduling Result

            dotted_line_in_file();
        }

        // 3-----------------------FCFS-----------------------------------
        fcfs_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of FCFS-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying FCFS-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of FCFS-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying FCFS-Scheduling Result

            dotted_line_in_file();
        }

        // 4-----------------------SJF-----------------------------------
        sjf_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of SJF-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying SJF-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of SJF-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying SJF-Scheduling Result

            dotted_line_in_file();
        }

        // 5-----------------------SRTN-----------------------------------
        srtn_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of SRTN-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying SRTN-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of SRTN-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying SRTN-Scheduling Result

            dotted_line_in_file();
        }

        // // 6-----------------------HRRN-----------------------------------
        hrrn_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of HRRN-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying HRRN-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of HRRN-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying HRRN-Scheduling Result

            dotted_line_in_file();
        }

        // 7-----------------------EDF-----------------------------------
        edf_scheduling(ps, n, final_result);
        if (result_on_terminal)
        {
            printf("\n-> Result of EDF-Scheduling");
            sort_by_index(ps, n);
            display2(ps, n); // displaying EDF-Scheduling Result
        }
        if (result_in_file)
        {
            fputs("\n\n-> Result of EDF-Scheduling", fptr_write);
            sort_by_index(ps, n);
            display2(ps, n); // displaying EDF-Scheduling Result

            dotted_line_in_file();
        }

        dotted_line();
        display_result(final_result);
        break;

    default:
        printf("\nWarning : Invalid Choice !!");
        break;
    }
    fclose(fptr_write);
}

void initialize_final_result(result *final_result)
{
    for (int i = 0; i < 8; i++)
    {
        final_result[i].context_switch = 0;
        final_result[i].throughput = 0;
        final_result[i].awt = 0;
        final_result[i].att = 0;
        final_result[i].art = 0;
    }
}

int generate_random_number(int lower, int upper)
{
    int range = upper - lower + 1;
    int randomNumber = rand() % range + lower;
    return randomNumber;
}

void set_processes(process *ps, int n)
{

    // generateProcesses(ps, n);

    ps[0].id = 1;
    ps[1].id = 2;
    ps[2].id = 3;

    ps[0].at = 1;
    ps[1].at = 2;
    ps[2].at = 3;

    ps[0].bt = 1;
    ps[1].bt = 2;
    ps[2].bt = 3;

    ps[0].period = 4;
    ps[1].period = 6;
    ps[2].period = 8;

    ps[0].no_of_execution = 6;
    ps[1].no_of_execution = 4;
    ps[2].no_of_execution = 3;

    // initializing with "0"
    for (int i = 0; i < n; i++)
    {
        ps[i].ct = 0;
        ps[i].tat = 0;
        ps[i].wt = 0;
        ps[i].rt = -1;
    }
}

void sort_by_arrival(process *ps, int n)
{
    process temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (ps[j].at > ps[j + 1].at)
            {
                temp = ps[j + 1];
                ps[j + 1] = ps[j];
                ps[j] = temp;
            }
        }
    }
}

void sort_by_index(process *ps, int no_of_process)
{
    process temp;
    for (int i = 0; i < no_of_process - 1; i++)
    {
        for (int j = 0; j < no_of_process - 1 - i; j++)
        {
            if (ps[j].id > ps[j + 1].id)
            {
                temp = ps[j + 1];
                ps[j + 1] = ps[j];
                ps[j] = temp;
            }
        }
    }
}

void sort_by_priority(process *ps, int n)
{
    process temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (ps[j].priority > ps[j + 1].priority)
            {
                temp = ps[j + 1];
                ps[j + 1] = ps[j];
                ps[j] = temp;
            }
        }
    }
}

void calculate_TAT_WT(process *ps, int no_of_process)
{
    for (int i = 0; i < no_of_process; i++)
    {
        ps[i].tat = ps[i].ct - ps[i].at;
        ps[i].wt = ps[i].tat - ps[i].bt;
    }
}

void calculate_AWT_ATT_ART(process *ps, int no_of_process, result *final_result, int index)
{
    double twt = 0, ttt = 0, trt = 0;
    for (int i = 0; i < no_of_process; i++)
    {
        twt += ps[i].wt;
        ttt += ps[i].tat;
        trt += ps[i].rt;
    }

    final_result[index].awt = twt / no_of_process;
    final_result[index].att = ttt / no_of_process;
    final_result[index].art = trt / no_of_process;
}

int get_process_index(process *ps, int no_of_process, int ticket_number)
{
    for (int i = 0; i < no_of_process; i++)
    {
        if (ticket_number >= ps[i].tickets[0] && ticket_number <= ps[i].tickets[1])
        {
            return i;
        }
    }
}

void display(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n------------------------------------------------------------------------\n");
        printf("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Priority  |  Tickets  |\n");
        printf("------------------------------------------------------------------------\n");
    }
    if (result_in_file)
    {
        dotted_line_in_file();
        fputs("\nProcesses are :", fptr_write);
        fputs("\n------------------------------------------------------------------------\n", fptr_write);
        fputs("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Priority  |  Tickets  |\n", fptr_write);
        fputs("------------------------------------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  | %3d -%3d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].priority, ps[i].tickets[0], ps[i].tickets[1]);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, "|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  | %3d -%3d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].priority, ps[i].tickets[0], ps[i].tickets[1]);
        }
    }

    if (result_on_terminal)
    {
        printf("------------------------------------------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("------------------------------------------------------------------------\n", fptr_write);
    }
}

void display2(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n------------------------------------------------------------------------------------------------------------------\n");
        printf("| S.No | ID  | Arrival-Time  | Brust-Time  | Completion-Time  | Turn-Around-Time | Waiting-Time | Response-Time  |\n");
        printf("------------------------------------------------------------------------------------------------------------------\n");
    }

    if (result_in_file)
    {

        fputs("\n------------------------------------------------------------------------------------------------------------------\n", fptr_write);
        fputs("| S.No | ID  | Arrival-Time  | Brust-Time  | Completion-Time  | Turn-Around-Time | Waiting-Time | Response-Time  |\n", fptr_write);
        fputs("------------------------------------------------------------------------------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("| %4d | %2d  | %12d  | %10d  | %15d  | %15f  | %11f  | %14f |\n",
                   i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
        }

        if (result_in_file)
        {
            fprintf(fptr_write, "| %4d | %2d  | %12d  | %10d  | %15d  | %15f  | %11f  | %14f |\n",
                    i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].ct, ps[i].tat, ps[i].wt, ps[i].rt);
        }
    }
    if (result_on_terminal)
    {
        printf("------------------------------------------------------------------------------------------------------------------\n");
    }
    if (result_in_file)
    {
        fputs("------------------------------------------------------------------------------------------------------------------\n", fptr_write);
    }
}

void display_Basic_process_details(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n-----------------------------------------------\n");
        printf("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |\n");
        printf("-----------------------------------------------\n");
    }
    if (result_in_file)
    {
        dotted_line_in_file();
        fputs("\nProcesses are :", fptr_write);
        fputs("\n-----------------------------------------------\n", fptr_write);
        fputs("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |\n", fptr_write);
        fputs("-----------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("|  %2d  |  %2d  |  %12d  |  %10d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, "|  %2d  |  %2d  |  %12d  |  %10d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt);
        }
    }

    if (result_on_terminal)
    {
        printf("-----------------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("-----------------------------------------------\n", fptr_write);
    }
}

void display_priority_process_details(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n------------------------------------------------------------\n");
        printf("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Priority  |\n");
        printf("------------------------------------------------------------\n");
    }
    if (result_in_file)
    {
        dotted_line_in_file();
        fputs("\nProcesses are :", fptr_write);
        fputs("\n------------------------------------------------------------\n", fptr_write);
        fputs("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Priority  |\n", fptr_write);
        fputs("------------------------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].priority);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, "|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].priority);
        }
    }

    if (result_on_terminal)
    {
        printf("------------------------------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("------------------------------------------------------------\n", fptr_write);
    }
}

void display_Lottery_process_details(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n------------------------------------------------------------\n");
        printf("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Tickets  |\n");
        printf("------------------------------------------------------------\n");
    }
    if (result_in_file)
    {
        dotted_line_in_file();
        fputs("\nProcesses are :", fptr_write);
        fputs("\n-----------------------------------------------------------\n", fptr_write);
        fputs("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Tickets  |\n", fptr_write);
        fputs("-----------------------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("|  %2d  |  %2d  |  %12d  |  %10d  | %3d -%3d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].tickets[0], ps[i].tickets[1]);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, "|  %2d  |  %2d  |  %12d  |  %10d  | %3d -%3d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].tickets[0], ps[i].tickets[1]);
        }
    }

    if (result_on_terminal)
    {
        printf("-----------------------------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("-----------------------------------------------------------\n", fptr_write);
    }
}

void display_EDF_details(process *ps, int n)
{
    if (result_on_terminal)
    {
        printf("\n---------------------------------------------------------------------------\n");
        printf("| S.No |  ID  |  Arrival-Time  | Period |  Burst-Time  | No. of Execution |\n");
        printf("---------------------------------------------------------------------------\n");
    }
    if (result_in_file)
    {
        dotted_line_in_file();
        fputs("\nProcesses are :", fptr_write);
        fputs("\n---------------------------------------------------------------------------\n", fptr_write);
        fputs("| S.No |  ID  |  Arrival-Time  | Period |  Burst-Time  | No. of Execution |\n", fptr_write);
        fputs("---------------------------------------------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("|  %2d  |  %2d  |  %12d  |  %4d  |  %10d  | %16d |\n", i + 1, ps[i].id, ps[i].at, ps[i].period, ps[i].bt, ps[i].no_of_execution);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, "|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  | %3d |\n", i + 1, ps[i].id, ps[i].at, ps[i].period, ps[i].bt, ps[i].no_of_execution);
        }
    }

    if (result_on_terminal)
    {
        printf("---------------------------------------------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("---------------------------------------------------------------------------\n", fptr_write);
    }
}

void round_robin(process *ps, int time_quantum, int no_of_process, result *final_result)
{
    sort_by_arrival(ps, no_of_process);
    int timeline = 0;
    int index = 0;
    int previous_process = -1;

    if (debug_RR)
    {
        printf("\nDebugging\n");
        printf("| Timeline  | ID | Remaining Burst Time| Response-Time | Completion Time \n");
    }

    while (1)
    {
        if (ps[index].rbt > 0)
        {
            if (ps[index].rt == -1)
            {
                ps[index].rt = timeline - ps[index].at;
            }

            if (previous_process == -1)
            {
                previous_process = ps[index].id;
            }
            else if (previous_process != ps[index].id)
            {
                final_result[0].context_switch++;
                previous_process = ps[index].id;
            }

            if (debug_RR)
            {

                printf("| (%2d - %2d) | %2d | %19d | %15f |", timeline, timeline + time_quantum, ps[index].id, ps[index].rbt, ps[index].rt);
            }

            if (ps[index].rbt > time_quantum)
            {
                timeline += time_quantum;
                ps[index].rbt -= time_quantum;
            }
            else
            {
                timeline += ps[index].rbt;
                ps[index].rbt = 0;
                ps[index].ct = timeline;
            }
            if (debug_RR)
            {
                printf(" %15d |\n", ps[index].ct);
            }
        }

        // Finding next process with RBT > 0
        int original_index = index;
        do
        {
            index = (index + 1) % no_of_process;
        } while (ps[index].rbt == 0 && original_index != index);

        // process changed
        // Checking if all processes are completed
        int completed_processes = 0;
        for (int i = 0; i < no_of_process; i++)
        {
            if (ps[i].rbt == 0)
            {
                completed_processes++;
            }
        }

        // breaking condition
        if (completed_processes == no_of_process)
        {
            break;
        }
    }
    final_result[0].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 0 : %d\n",timeline);

    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 0);
    // printf("\nART RR : %f", final_result[0].art);
}

void priority_scheduling(process *ps, int no_of_process, result *final_result)
{
    // set_processes(ps, no_of_process);
    set_RBT_RT(ps, no_of_process);
    sort_by_priority(ps, no_of_process);
    int timeline = 0;
    int index = 0;
    int previous_process = -1;

    if (debug_PRIORITY)
    {
        printf("\nDebugging\n");
        printf("| Timeline  | ID | Priority | Remaining Burst Time | Completion Time | Context-Switch |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        if (ps[index].at <= timeline && ps[index].rbt > 0)
        {
            if (ps[index].rt == -1)
            {
                ps[index].rt = timeline - ps[index].at;
            }
            if (previous_process == -1)
            {
                previous_process = ps[index].id;
            }
            else if (previous_process != ps[index].id)
            {
                final_result[1].context_switch++;
                previous_process = ps[index].id;
            }

            timeline += ps[index].rbt;
            ps[index].rbt = 0;
            ps[index].ct = timeline;
            completed_processes++;

            if (debug_PRIORITY)
            {
                printf("| (%2d - %2d) | %2d | %8d | %19d | %19d | %19d |\n",
                       timeline - ps[index].bt, timeline, ps[index].id, ps[index].priority, ps[index].bt, ps[index].ct, final_result[1].context_switch);
            }
            index = 0;
        }
        else
        {
            int original_index = index;
            do
            {
                index = (index + 1) % no_of_process;
            } while (ps[index].rbt == 0 && original_index != index && ps[index].at > timeline);
        }
    }
    final_result[1].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 1 : %d\n",timeline);
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 1);
    // printf("\nART Priority : %f", final_result[1].art);
}

void lottery_scheduling(process *ps, int time_quantum, int no_of_process, result *final_result)
{
    set_RBT_RT(ps, no_of_process);
    sort_by_index(ps, no_of_process);
    int timeline = 0;
    int index = 0;
    int ticket_number = 0;
    int local_tq = 0; // local variable to keep track of time spent on the current process
    int previous_process = -1;
    int ticket_range = ps[no_of_process - 1].tickets[1];
    if (debug_LOTTERY)
    {
        printf("\nDebugging Lottery Scheduling\n");
        printf("| Timeline  | ID | Remaining Burst Time | Completion Time |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        ticket_number = generate_random_number(1, ticket_range);     // generating random no. between 1 - ticket range
        index = get_process_index(ps, no_of_process, ticket_number); // will give index of process by ticket number

        if (previous_process == -1)
        {
            previous_process = ps[index].id;
        }
        else if (previous_process != ps[index].id)
        {
            final_result[2].context_switch++;
            previous_process = ps[index].id;
        }

        if (ps[index].at <= timeline && ps[index].rbt > 0)
        {
            if (ps[index].rt == -1)
            {
                ps[index].rt = timeline - ps[index].at;
            }

            if (ps[index].rbt > time_quantum)
            {
                local_tq = time_quantum;
                timeline += time_quantum;
                ps[index].rbt -= time_quantum;
                ps[index].ct = timeline;
            }
            else
            {
                local_tq = ps[index].rbt;
                timeline += ps[index].rbt;
                ps[index].rbt = 0;
                ps[index].ct = timeline;
                completed_processes++;
            }

            if (debug_LOTTERY)
            {
                printf("| (%2d - %2d) | %2d | %19d | %19d |\n",
                       timeline - local_tq, timeline, ps[index].id, ps[index].rbt, ps[index].ct);
            }
        }
    }
    final_result[2].throughput = (1.0) * no_of_process / timeline;
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 2);
    // printf("\nART Lottery : %f", final_result[2].art);
}

void fcfs_scheduling(process *ps, int no_of_process, result *final_result)
{
    set_RBT_RT(ps, no_of_process);
    sort_by_arrival(ps, no_of_process); // sorted acc. to arrival time
    int previous_process = -1;
    int timeline = 0;
    int index = 0;
    if (debug_FCFS)
    {
        printf("\nDebugging FCFS Scheduling\n");
        printf("| Timeline  | ID | Remaining Burst Time | Completion Time |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        if (ps[index].rt == -1)
        {
            ps[index].rt = timeline - ps[index].at;
        }
        if (previous_process == -1)
        {
            previous_process = ps[index].id;
        }
        else if (previous_process != ps[index].id)
        {
            final_result[3].context_switch++;
            previous_process = ps[index].id;
        }
        if (ps[index].at <= timeline && ps[index].rbt > 0)
        {
            timeline += ps[index].rbt;
            ps[index].rbt = 0;
            ps[index].ct = timeline;
            completed_processes++;

            if (debug_FCFS)
            {
                printf("| (%2d - %2d) | %2d | %19d | %19d |\n",
                       timeline - ps[index].bt, timeline, ps[index].id, ps[index].bt, ps[index].ct);
            }
        }
        else
        {
            printf("\nNote : Arrival time is greater than timeline !");
        }
        index++;
    }
    final_result[3].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 3 : %d\n",timeline);
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 3);
}

void sjf_scheduling(process *ps, int no_of_process, result *final_result)
{
    set_RBT_RT(ps, no_of_process);
    sort_by_arrival(ps, no_of_process); // sorted acc. to arrival time

    int timeline = 0;
    int index = 0;
    int previous_process = -1;
    if (debug_SJF)
    {
        printf("\nDebugging SJF Scheduling\n");
        printf("| Timeline  | ID | Remaining Burst Time | Completion Time |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        int shortest_job_index = -1;
        int shortest_job_time = INT_MAX;

        for (int i = 0; i < no_of_process; i++)
        {
            if (ps[i].at <= timeline && ps[i].rbt > 0 && ps[i].rbt < shortest_job_time)
            {
                shortest_job_time = ps[i].rbt;
                shortest_job_index = i;
            }
        }

        if (shortest_job_index != -1)
        {

            if (debug_SJF)
            {
                printf("| (%2d - %2d) | %2d | %19d |", timeline, timeline + ps[shortest_job_index].rbt,
                       ps[shortest_job_index].id, ps[shortest_job_index].rbt);
            }
            if (ps[shortest_job_index].rt == -1)
            {
                ps[shortest_job_index].rt = timeline - ps[shortest_job_index].at;
            }
            if (previous_process == -1)
            {
                previous_process = ps[shortest_job_time].id;
            }
            else if (previous_process != ps[shortest_job_time].id)
            {
                final_result[4].context_switch++;
                previous_process = ps[index].id;
            }

            timeline += ps[shortest_job_index].rbt;
            ps[shortest_job_index].rbt = 0;
            ps[shortest_job_index].ct = timeline;
            completed_processes++;

            if (debug_SJF)
            {
                printf(" %19d |\n", ps[shortest_job_index].ct);
            }
        }
        else
        {
            timeline++;
        }
    }

    final_result[4].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 4 : %d\n",timeline);
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 4);
}

void srtn_scheduling(process *ps, int no_of_process, result *final_result)
{
    set_RBT_RT(ps, no_of_process);
    sort_by_arrival(ps, no_of_process); // sorted acc. to arrival time
    int previous_process = -1;
    int timeline = 0;
    int index = 0;

    if (debug_SRTN)
    {
        printf("\nDebugging SJF Scheduling\n");
        printf("| Timeline  | ID | Remaining Burst Time | Completion Time |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        int shortest_job_index = -1;
        int shortest_job_time = INT_MAX;

        // Find the process with the shortest remaining burst time among the arrived processes
        for (int i = 0; i < no_of_process; i++)
        {
            if (ps[i].at <= timeline && ps[i].rbt > 0 && ps[i].rbt < shortest_job_time)
            {
                shortest_job_time = ps[i].rbt;
                shortest_job_index = i;
            }
        }

        if (shortest_job_index != -1)
        {
            if (debug_SRTN)
            {
                printf("| (%2d - %2d) | %2d | %19d |", timeline, timeline + 1, ps[shortest_job_index].id, ps[shortest_job_index].rbt);
            }
            if (ps[shortest_job_index].rt == -1)
            {
                ps[shortest_job_index].rt = timeline - ps[shortest_job_index].at;
            }
            timeline++;
            ps[shortest_job_index].rbt--;

            if (previous_process == -1)
            {
                previous_process = ps[index].id;
            }
            else if (previous_process != ps[shortest_job_index].id)
            {
                final_result[5].context_switch++;
                previous_process = ps[shortest_job_index].id;
            }
            // Checking if the process has completed
            if (ps[shortest_job_index].rbt == 0)
            {
                ps[shortest_job_index].ct = timeline;
                completed_processes++;

                // Print completion details
                if (debug_SRTN)
                {
                    printf(" %19d |\n", ps[shortest_job_index].ct);
                }
            }
            else
            {
                if (debug_SRTN)
                {
                    printf("\n");
                }
            }
        }
        else
        {
            // No process is available, just increment timeline
            timeline++;
        }
    }
    final_result[5].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 5 : %d\n",timeline);
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 5);
}

void hrrn_scheduling(process *ps, int no_of_process, result *final_result)
{
    set_RBT_RT(ps, no_of_process);
    sort_by_arrival(ps, no_of_process); // sorted acc. to arrival time
    int previous_process = -1;
    int timeline = 0;

    if (debug_HRRN)
    {
        printf("\nDebugging HRRN Scheduling\n");
        printf("| Timeline  | ID | Remaining Burst Time | Completion Time |\n");
    }

    int completed_processes = 0;
    while (completed_processes < no_of_process)
    {
        int selected_process_index = -1;
        double highest_response_ratio = -1;

        for (int i = 0; i < no_of_process; i++)
        {
            if (ps[i].at <= timeline && ps[i].rbt > 0)
            {
                double response_ratio = (timeline - ps[i].at + ps[i].bt) / (double)ps[i].bt;

                if (response_ratio > highest_response_ratio)
                {
                    highest_response_ratio = response_ratio;
                    selected_process_index = i;
                }
            }
        }

        if (selected_process_index != -1)
        {
            if (debug_HRRN)
            {

                printf("| (%2d - %2d) | %2d | %19d |\n", timeline, timeline + ps[selected_process_index].rbt,
                       ps[selected_process_index].id, ps[selected_process_index].rbt);
            }

            if (ps[selected_process_index].rt == -1)
            {
                ps[selected_process_index].rt = timeline - ps[selected_process_index].at;
            }
            if (previous_process == -1)
            {
                previous_process = ps[selected_process_index].id;
            }
            else if (previous_process != ps[selected_process_index].id)
            {
                final_result[6].context_switch++;
                previous_process = ps[selected_process_index].id;
            }
            timeline += ps[selected_process_index].rbt;
            ps[selected_process_index].rbt = 0;
            ps[selected_process_index].ct = timeline;
            completed_processes++;

            if (debug_HRRN)
            {
                printf(" %19d |\n", ps[selected_process_index].ct);
            }
        }
        else
        {
            timeline++;
        }
    }
    final_result[6].throughput = (1.0) * no_of_process / timeline;
    // printf("\nTimeline 6 : %d\n",timeline);
    calculate_TAT_WT(ps, no_of_process);
    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 6);
}

void set_current_deadline_AT(process *ps, int n)
{
    for (int i = 0; i < n; i++)
    {
        ps[i].at = 0;
        ps[i].current_deadline = ps[i].period;
    }
}

void sort_by_deadline(process *ps, int n)
{
    process temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (ps[j].current_deadline > ps[j + 1].current_deadline)
            {
                temp = ps[j + 1];
                ps[j + 1] = ps[j];
                ps[j] = temp;
            }
        }
    }
}

void set_Execution_buffer(process *ps, int n)
{
    for (int i = 0; i < n; i++)
    {
        ps[i].no_of_execution_buffer = ps[i].no_of_execution;
    }
}

void edf_scheduling(process *ps, int no_of_process, result *final_result)
{
    int timeline = 0;

    if (debug_EDF)
    {
        printf("\nDebugging EDF Scheduling\n");
        printf("| Timeline  | ID | (AT, CD) | Done |\n");
    }

    int completed_processes = 0;
    int i = 0, t1, t2;
    while (completed_processes < no_of_process)
    {
        sort_by_deadline(ps, no_of_process);
        i = 0;
    label:
        t1 = timeline;
        if (debug_EDF)
        {
            // printf("\nWorking on ID : %d\n", ps[i].id);
        }

        if (ps[i].at <= timeline && ps[i].no_of_execution_buffer > 0)
        {
            int at = ps[i].at;
            ps[i].at = ps[i].current_deadline;
            ps[i].current_deadline += ps[i].period;
            timeline += ps[i].bt;
            t2 = timeline;
            ps[i].no_of_execution_buffer--;

            if (ps[i].no_of_execution_buffer == 0)
            {
                completed_processes++;
            }

            ps[i].tat += (1.0) * (t2 - at) / ps[i].no_of_execution;
            ps[i].wt += (1.0) * (t1 - at) / ps[i].no_of_execution;
            ps[i].rt = ps[i].wt;

            if (debug_EDF)
            {
                printf("| %3d -%3d  | %2d | %2d - %2d  | %4d |\n", t1, t2, ps[i].id, ps[i].at, ps[i].current_deadline, completed_processes);
            }
        }
        else
        {
            i++;
            goto label;
        }
    }

    calculate_AWT_ATT_ART(ps, no_of_process, final_result, 7);
    // final_result[7].context_switch = no_of_process - 1;
}

void display_result(result *final_result)
{
    if (result_on_terminal)
    {
        dotted_line();
        printf("\n\nFinal Comparison");
        display_AWT(final_result, 8);
        dotted_line();
        display_ATT(final_result, 8);
        dotted_line();
        display_ART(final_result, 8);
        dotted_line();
        display_Context_switch(final_result, 8);
        dotted_line();
        display_throughput(final_result, 8);
        dotted_line();
    }

    if (result_in_file)
    {
        fputs("\n\nFinal Comparison", fptr_write);
        display_AWT(final_result, 8);
        dotted_line_in_file();
        display_ATT(final_result, 8);
        dotted_line_in_file();
        display_ART(final_result, 8);
        dotted_line_in_file();
        display_Context_switch(final_result, 8);
        dotted_line_in_file();
        display_throughput(final_result, 8);
        dotted_line_in_file();
    }
}

void display_AWT(result *final_result, int n)
{
    double max = -1;
    for (int i = 0; i < n; i++)
    {
        if (final_result[i].awt > max)
        {
            max = final_result[i].awt;
        }
    }

    double scale = 100 / max;
    // printf("\nScale : %0.2f", scale);

    if (result_on_terminal)
    {
        printf("\n--------------------------------\n");
        printf("Average Waiting Time Comparison\n");
        printf("--------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("\n--------------------------------\n", fptr_write);
        fputs("Average Waiting Time Comparison\n", fptr_write);
        fputs("--------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("\n");
            if (i == 0)
                printf("Round-Robin |");
            else if (i == 1)
                printf("Priority    |");
            else if (i == 2)
                printf("Lottery     |");
            else if (i == 3)
                printf("FCFS        |");
            else if (i == 4)
                printf("SJF         |");
            else if (i == 5)
                printf("SRTN        |");
            else if (i == 6)
                printf("HRRN        |");
            else if (i == 7)
                printf("EDF         |");
        }
        if (result_in_file)
        {
            fputs("\n", fptr_write);
            if (i == 0)
                fputs("Round-Robin |", fptr_write);
            else if (i == 1)
                fputs("Priority    |", fptr_write);
            else if (i == 2)
                fputs("Lottery     |", fptr_write);
            else if (i == 3)
                fputs("FCFS        |", fptr_write);
            else if (i == 4)
                fputs("SJF         |", fptr_write);
            else if (i == 5)
                fputs("SRTN        |", fptr_write);
            else if (i == 6)
                fputs("HRRN        |", fptr_write);
            else if (i == 7)
                fputs("EDF         |", fptr_write);
        }

        int awtStars = (int)(scale * final_result[i].awt);
        // printf("\nStars : %d", awtStars);
        for (int j = 0; j < awtStars; j++)
        {
            if (result_on_terminal)
            {
                printf("#");
            }
            if (result_in_file)
            {
                fputs("#", fptr_write);
            }
        }

        if (result_on_terminal)
        {
            printf(" (%.2f)\n", final_result[i].awt);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, " (%.2lf)\n", final_result[i].awt);
        }
    }
}

void display_ATT(result *final_result, int n)
{
    double max = -1;
    for (int i = 0; i < n; i++)
    {
        if (final_result[i].att > max)
        {
            max = final_result[i].att;
        }
    }

    double scale = 100 / max;
    if (result_on_terminal)
    {
        printf("\n------------------------------------\n");
        printf("Average Turn-Around Time Comparison\n");
        printf("------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("\n------------------------------------\n", fptr_write);
        fputs("Average Turn-Around Time Comparison\n", fptr_write);
        fputs("------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("\n");
            if (i == 0)
                printf("Round-Robin |");
            else if (i == 1)
                printf("Priority    |");
            else if (i == 2)
                printf("Lottery     |");
            else if (i == 3)
                printf("FCFS        |");
            else if (i == 4)
                printf("SJF         |");
            else if (i == 5)
                printf("SRTN        |");
            else if (i == 6)
                printf("HRRN        |");
            else if (i == 7)
                printf("EDF         |");
        }
        if (result_in_file)
        {
            fputs("\n", fptr_write);
            if (i == 0)
                fputs("Round-Robin |", fptr_write);
            else if (i == 1)
                fputs("Priority    |", fptr_write);
            else if (i == 2)
                fputs("Lottery     |", fptr_write);
            else if (i == 3)
                fputs("FCFS        |", fptr_write);
            else if (i == 4)
                fputs("SJF         |", fptr_write);
            else if (i == 5)
                fputs("SRTN        |", fptr_write);
            else if (i == 6)
                fputs("HRRN        |", fptr_write);
            else if (i == 7)
                fputs("EDF         |", fptr_write);
        }

        int attStars = (int)(scale * final_result[i].att);

        for (int j = 0; j < attStars; j++)
        {
            if (result_on_terminal)
            {
                printf("#");
            }
            if (result_in_file)
            {
                fputs("#", fptr_write);
            }
        }
        if (result_on_terminal)
        {
            printf(" (%.2lf)\n", final_result[i].att);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, " (%.2lf)\n", final_result[i].att);
        }
    }
}

void display_ART(result *final_result, int n)
{
    double max = -1;
    for (int i = 0; i < n; i++)
    {
        if (final_result[i].art > max)
        {
            max = final_result[i].art;
        }
    }

    double scale = 100 / max;
    if (result_on_terminal)
    {
        printf("\n------------------------------------\n");
        printf("Average Response-Time Comparison\n");
        printf("------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("\n------------------------------------\n", fptr_write);
        fputs("Average Response-Time Comparison\n", fptr_write);
        fputs("------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("\n");
            if (i == 0)
                printf("Round-Robin |");
            else if (i == 1)
                printf("Priority    |");
            else if (i == 2)
                printf("Lottery     |");
            else if (i == 3)
                printf("FCFS        |");
            else if (i == 4)
                printf("SJF         |");
            else if (i == 5)
                printf("SRTN        |");
            else if (i == 6)
                printf("HRRN        |");
            else if (i == 7)
                printf("EDF         |");
        }
        if (result_in_file)
        {
            fputs("\n", fptr_write);
            if (i == 0)
                fputs("Round-Robin |", fptr_write);
            else if (i == 1)
                fputs("Priority    |", fptr_write);
            else if (i == 2)
                fputs("Lottery     |", fptr_write);
            else if (i == 3)
                fputs("FCFS        |", fptr_write);
            else if (i == 4)
                fputs("SJF         |", fptr_write);
            else if (i == 5)
                fputs("SRTN        |", fptr_write);
            else if (i == 6)
                fputs("HRRN        |", fptr_write);
            else if (i == 7)
                fputs("EDF         |", fptr_write);
        }

        int attStars = (int)(scale * final_result[i].art);

        for (int j = 0; j < attStars / 2; j++)
        {
            if (result_on_terminal)
            {
                printf("#");
            }
            if (result_in_file)
            {
                fputs("#", fptr_write);
            }
        }
        if (result_on_terminal)
        {
            printf(" (%.2lf)\n", final_result[i].art);
        }
        if (result_in_file)
        {
            fprintf(fptr_write, " (%.2lf)\n", final_result[i].art);
        }
    }
}

void display_Context_switch(result *final_result, int n)
{
    double max = -1;
    for (int i = 0; i < n; i++)
    {
        if (final_result[i].context_switch > max)
        {
            max = final_result[i].context_switch;
        }
    }

    double scale = 100 / max;
    if (result_on_terminal)
    {
        printf("\n------------------------------------\n");
        printf("Context-Switch Comparison\n");
        printf("------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("\n------------------------------------\n", fptr_write);
        fputs("Context-Switch Comparison\n", fptr_write);
        fputs("------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("\n");
            if (i == 0)
                printf("Round-Robin |");
            else if (i == 1)
                printf("Priority    |");
            else if (i == 2)
                printf("Lottery     |");
            else if (i == 3)
                printf("FCFS        |");
            else if (i == 4)
                printf("SJF         |");
            else if (i == 5)
                printf("SRTN        |");
            else if (i == 6)
                printf("HRRN        |");
            else if (i == 7)
                printf("EDF         |");
        }
        if (result_in_file)
        {
            fputs("\n", fptr_write);
            if (i == 0)
                fputs("Round-Robin |", fptr_write);
            else if (i == 1)
                fputs("Priority    |", fptr_write);
            else if (i == 2)
                fputs("Lottery     |", fptr_write);
            else if (i == 3)
                fputs("FCFS        |", fptr_write);
            else if (i == 4)
                fputs("SJF         |", fptr_write);
            else if (i == 5)
                fputs("SRTN        |", fptr_write);
            else if (i == 6)
                fputs("HRRN        |", fptr_write);
            else if (i == 7)
                fputs("EDF         |", fptr_write);
        }

        int attStars = (scale * final_result[i].context_switch);

        for (int j = 0; j < attStars; j++)
        {
            if (result_on_terminal)
            {
                printf("#");
            }
            if (result_in_file)
            {
                fputs("#", fptr_write);
            }
        }

        if (result_on_terminal)
        {
            printf(" (%d)\n", final_result[i].context_switch);
        }

        if (result_in_file)
        {
            fprintf(fptr_write, " (%d)\n", final_result[i].context_switch);
        }
    }
}

void display_throughput(result *final_result, int n)
{
    double max = -1;
    for (int i = 0; i < n; i++)
    {
        if (final_result[i].throughput > max)
        {
            max = final_result[i].throughput;
        }
    }

    double scale = 100 / max;
    if (result_on_terminal)
    {
        printf("\n------------------------------------\n");
        printf("Through-Put Comparison\n");
        printf("------------------------------------\n");
    }

    if (result_in_file)
    {
        fputs("\n------------------------------------\n", fptr_write);
        fputs("Through-Put Comparison\n", fptr_write);
        fputs("------------------------------------\n", fptr_write);
    }

    for (int i = 0; i < n; i++)
    {
        if (result_on_terminal)
        {
            printf("\n");
            if (i == 0)
                printf("Round-Robin |");
            else if (i == 1)
                printf("Priority    |");
            else if (i == 2)
                printf("Lottery     |");
            else if (i == 3)
                printf("FCFS        |");
            else if (i == 4)
                printf("SJF         |");
            else if (i == 5)
                printf("SRTN        |");
            else if (i == 6)
                printf("HRRN        |");
            else if (i == 7)
                printf("EDF         |");
        }
        if (result_in_file)
        {
            fputs("\n", fptr_write);
            if (i == 0)
                fputs("Round-Robin |", fptr_write);
            else if (i == 1)
                fputs("Priority    |", fptr_write);
            else if (i == 2)
                fputs("Lottery     |", fptr_write);
            else if (i == 3)
                fputs("FCFS        |", fptr_write);
            else if (i == 4)
                fputs("SJF         |", fptr_write);
            else if (i == 5)
                fputs("SRTN        |", fptr_write);
            else if (i == 6)
                fputs("HRRN        |", fptr_write);
            else if (i == 7)
                fputs("EDF         |", fptr_write);
        }

        int attStars = (scale * final_result[i].throughput);

        for (int j = 0; j < attStars; j++)
        {
            if (result_on_terminal)
            {
                printf("#");
            }
            if (result_in_file)
            {
                fputs("#", fptr_write);
            }
        }

        if (result_on_terminal)
        {
            printf(" (%0.4f)\n", final_result[i].throughput);
        }

        if (result_in_file)
        {
            fprintf(fptr_write, " (%0.4f)\n", final_result[i].throughput);
        }
    }
}

void separate_results(result *final_result, int i)
{
    double max;
    if (final_result[i].awt > final_result[i].att && final_result[i].awt > final_result[i].art)
    {
        max = final_result[i].awt;
    }
    else if (final_result[i].att > final_result[i].art)
    {
        max = final_result[i].att;
    }
    else
    {
        max = final_result[i].art;
    }

    double scale = 100 / max;
    // printf("Scale : %f", scale);
    // Average WT
    if (result_on_terminal)
    {
        printf("\nAverage-Waiting-Time     |");
    }
    if (result_in_file)
    {
        fputs("\nAverage-Waiting-Time     |", fptr_write);
    }

    int attStars = (int)((scale * final_result[i].awt));
    for (int j = 0; j < attStars; j++)
    {
        if (result_on_terminal)
        {
            printf("#");
        }
        if (result_in_file)
        {
            fputs("#", fptr_write);
        }
    }

    // Average TAT
    if (result_on_terminal)
    {
        printf(" (%0.2f)\n", final_result[i].awt);
        printf("\nAverage-Turn-Around-Time |");
    }
    if (result_in_file)
    {
        fprintf(fptr_write, " (%0.2f)\n", final_result[i].awt);
        fputs("\nAverage-Turn-Around-Time |", fptr_write);
    }
    attStars = (int)(scale * final_result[i].att);
    for (int j = 0; j < attStars; j++)
    {
        if (result_on_terminal)
        {
            printf("#");
        }
        if (result_in_file)
        {
            fputs("#", fptr_write);
        }
    }

    // Average RT
    if (result_on_terminal)
    {
        printf(" (%0.2f)\n", final_result[i].att);
        printf("\nAverage-Response-Time    |");
    }
    if (result_in_file)
    {
        fprintf(fptr_write, " (%0.2f)\n", final_result[i].att);
        fputs("\nAverage-Response-Time    |", fptr_write);
    }
    attStars = (int)ceil(scale * final_result[i].art);
    for (int j = 0; j < attStars; j++)
    {
        if (result_on_terminal)
        {
            printf("#");
        }
        if (result_in_file)
        {
            fputs("#", fptr_write);
        }
    }

    // Context Switch
    if (result_on_terminal)
    {
        printf(" (%0.2f)\n", final_result[i].art);
        printf("\nContext-Switch           |");
    }
    if (result_in_file)
    {
        fprintf(fptr_write, " (%0.2f)\n", final_result[i].art);
        fputs("\nContext-Switches         |", fptr_write);
    }

    if (result_on_terminal)
    {
        printf(" (%d)\n", final_result[i].context_switch);
    }
    if (result_in_file)
    {
        fprintf(fptr_write, " (%d)\n", final_result[i].context_switch);
    }

    // Through-put
    if (result_on_terminal)
    {
        printf("\nThrough-Put              |");
    }
    if (result_in_file)
    {
        fputs("\nThrough-Put Value        |", fptr_write);
    }
    if (result_on_terminal)
    {
        printf(" (%0.4f)\n", final_result[i].throughput);
    }
    if (result_in_file)
    {
        fprintf(fptr_write, " (%0.4f)\n", final_result[i].throughput);
    }
}

void dotted_line()
{
    for (int i = 0; i < 70; i++)
    {
        printf("__");
    }
}

void dotted_line_in_file()
{
    for (int i = 0; i < 57; i++)
    {
        fputs("__", fptr_write);
    }
}

void generateProcesses(process *processes, int n)
{
    int index = generate_random_number(0, n);
    processes[index].at = 0; // giving at = 0 to any process randomly

    for (int i = 0; i < n; i++)
    {
        processes[i].id = i + 1;

        if (i == index)
        {
            continue;
        }

        processes[i].at = generate_random_number(0, 9); // Random arrival time between 0 and 9
    }

    int max_of_arrival = 0;
    for (int i = 0; i < n; i++)
    {
        if (max_of_arrival < processes[i].at)
        {
            max_of_arrival = processes[i].at;
        }
    }

    for (int i = 0; i < n; i++)
    {
        processes[i].bt = generate_random_number(max_of_arrival, 16); // Random burst time between 1 and 16
        processes[i].rbt = processes[i].bt;
    }
}

void generate_tickets(process *processes, int n)
{
    int lower = 1;
    processes[0].tickets[0] = lower;

    int upper = generate_random_number(lower + 1, lower + 20);
    processes[0].tickets[1] = upper;

    for (int i = 1; i < n; i++)
    {
        lower = processes[i - 1].tickets[1] + 1;
        processes[i].tickets[0] = lower;

        upper = generate_random_number(lower + 1, lower + 20);
        processes[i].tickets[1] = generate_random_number(lower + 1, upper);
    }

    int ticket_range = processes[n - 1].tickets[1];
}

void set_RBT_RT(process *ps, int n)
{
    for (int i = 0; i < n; i++)
    {
        ps[i].rbt = ps[i].bt;
        ps[i].rt = -1;
    }
}

void set_Priority(process *ps, int n)
{
    int priority;
    int priority_given = 0;
    int total_priority = n;
    int flag;

    int j = 0;
    while (priority_given < total_priority)
    {
        flag = 0;

        priority = generate_random_number(1, n);

        for (int i = 0; i < j; i++)
        {
            if (ps[i].priority == priority)
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            ps[j].priority = priority;
            // printf("\nProcess %d Priority: %d", ps[j].id, priority);
            priority_given++;
            j++;
        }
    }
}