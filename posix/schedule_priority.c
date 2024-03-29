#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "schedule_priority.h"

Node *task_list = NULL;
Node *sorted_task_list = NULL;
// this list will contains all tasks that sorted in running order(based on lexicographical order)


void priority_scheduler(){
    int time_counting = 0;
    int total_dispatcher_time = 0;
    copyList(&sorted_task_list, task_list);
    sortList(sorted_task_list);
    // Node *cur = running_order_list;
    while(1){
        Task* t = pickNextTask(); 
        if(t == NULL) break;
        delete(&task_list, t); //delete slected task

        t->start_time = time_counting;
        
        run(t, t->burst); //run this task
        time_counting += t->burst; //update time counting after run this task
        printf("        Time is now: %d\n", time_counting);
        t->complete_time = time_counting; //update complete time = time_counting
        
        //move to next task
        if(pickNextTask()){ //there are tasks to switch to
            total_dispatcher_time++;
        }

    }

    printf("CPU Utilization: %.2f%%\n",  time_counting * 100.0 / (time_counting + total_dispatcher_time));
    runTaskInfor(sorted_task_list);
}

bool comesBefore(Task* t1, Task* t2){
    if(t1->priority != t2->priority) return t1->priority > t2->priority;
    return strcmp(t1->name, t2->name) < 0;
}

void add(char* name, int priority, int burst){
    Task *task = createTask(name, priority, burst);
    insert(&task_list, task);
}

void schedule(){
    priority_scheduler();
}

Task *pickNextTask(){
    if(task_list == NULL) return NULL;
    Node *cur = task_list;
    Task *best_sofar = cur->task;
    while(cur != NULL){
        if(comesBefore(cur->task, best_sofar)){
            best_sofar = cur->task;
        }
        
        cur = cur->next;
    }

    return best_sofar;
}

