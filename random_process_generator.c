#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct process
{
    int id;         // process id
    int bt;         // burst time
    int rbt;        // remaining bt
    int at;         // arrival time
    int ct;         // completion time
    int wt;         // waiting time
    int tat;        // turn-around time
    int priority;   // for priority scheduling algorithm
    int tickets[2]; // to maintain tickets {0:lower-ticket, 1:upper-ticket}
    int rt;         // response-time
};
typedef struct process process;

int generate_random_number(int lower, int upper)
{
    int range = upper - lower + 1;
    int randomNumber = rand() % range + lower;
    return randomNumber;
}

void sort_processes(process *ps, int n)
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

void generate_tickets(process *processes, int n)
{
    int lower = 1;
    processes[0].tickets[0] = lower;

    int upper = generate_random_number(lower + 1, lower + 20);
    processes[0].tickets[1] = upper;
    // printf("Testing : (%d,%d)", lower, upper);
    for (int i = 1; i < n; i++)
    {
        lower = processes[i - 1].tickets[1] + 1;
        processes[i].tickets[0] = lower;

        upper = generate_random_number(lower + 1, lower + 20);
        processes[i].tickets[1] = generate_random_number(lower + 1, upper);
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nProcess %d : (%d,%d)", i + 1, processes[i].tickets[0], processes[i].tickets[1]);
    }
    int ticket_range = processes[n - 1].tickets[1];
    printf("\n\nTicket Range : (1,%d)", ticket_range);
}

void generateProcesses(process *processes, int n)
{

    int index = generate_random_number(0, n);
    // printf("\nFor arrival time 0 : %d", index+1);
    processes[index].at = 0;

    for (int i = 0; i < n; i++)
    {
        processes[i].id = i + 1;
        processes[i].ct = 0;
        processes[i].wt = 0;
        processes[i].tat = 0;
        processes[i].rt = -1;
        processes[i].priority = generate_random_number(1, n + 1);

        if (i == index)
        {
            continue;
        }
        processes[i].at = generate_random_number(0, n); // Random arrival time between 0 and n
    }

    int max_of_arrival = 0;
    for (int i = 0; i < n; i++)
    {
        if (max_of_arrival < processes[i].at)
        {
            max_of_arrival = processes[i].at;
        }
    }
    // printf("\nMax of arrival time : %d", max_of_arrival);
    for (int i = 0; i < n; i++)
    {
        processes[i].bt = generate_random_number(max_of_arrival / 2, 16); // Random burst time between max of arrival / 2 and 16
        processes[i].rbt = processes[i].bt;
    }

    generate_tickets(processes, n);
}

void display(process *ps, int n)
{
    printf("\n------------------------------------------------------------------------\n");
    printf("| S.No |  ID  |  Arrival-Time  |  Burst-Time  |  Priority  |  Tickets  |\n");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("|  %2d  |  %2d  |  %12d  |  %10d  |  %8d  |  %7d  |\n", i + 1, ps[i].id, ps[i].at, ps[i].bt, ps[i].priority, (ps[i].tickets[1] - ps[i].tickets[0] + 1));
    }
    printf("------------------------------------------------------------------------\n");
}

int main()
{
    srand(time(NULL));
    int n;
    printf("\nEnter No. of process : ");
    scanf("%d", &n);

    process processes[n];

    generateProcesses(processes, n);
    sort_processes(processes, n);
    printf("\n\nGenerated processes are : \n");
    display(processes, n);

    int sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += processes[i].tickets[1] - processes[i].tickets[0] + 1;
    }
        printf("\nDistributed tickets are : %d", sum);
    return 0;
}
