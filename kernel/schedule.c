#include <schedule.h>
#include <irq.h>
#include <printf.h>

static struct task_struct init_task = INIT_TASK;
struct task_struct* current = &(init_task);
struct task_struct* initial_task = &(init_task);
int nr_tasks = 1;

void preempt_disable(){
    current->preempt_count++;
}

void preempt_enable(){
    current->preempt_count--;
}

void _schedule(){
    preempt_disable();
    int c;
    struct task_struct* p;
    struct task_struct* next_task;
    while (1){
        c = -1;
        for (p = initial_task; p; p = p->next_task){
            if (p->state == TASK_RUNNING && p->counter > c){
                c = p->counter;
                next_task = p;  //point to next task
            }
        }
        if (c){
            break;
        }
        for (p = initial_task; p; p = p->next_task){
            p->counter = (p->counter >> 1) + p->priority;
        }
    }
    switch_to(next_task);
    preempt_enable();
}

void schedule(){
    current->counter = 0;
    _schedule();
}

void switch_to(struct task_struct* next){
	struct task_struct * p;
	printf("\n\r\n\r----------- Task switch -----------\r\n");
	for(p = initial_task; p; p = p->next_task) {
		printf("\n\rtask[%d] counter = %d\n\r", p->pid, p->counter);
		printf("task[%d] priority = %d\n\r", p->pid, p->priority);
		printf("task[%d] preempt_count = %d\n\r", p->pid, p->preempt_count);
		printf("task[%d] sp = 0x%08x\n\r", p->pid, p->cpu_context.sp);
		printf("\n\r------------------------------\r\n");
	}
	printf("\n\rtask output: ");

    if (current == next){
        return;
    }
    struct task_struct* prev = current;
    current = next;
    cpu_switch_to(prev, next);
}

void schedule_tail(){
    preempt_enable();
}

void timer_tick(){
    current->counter--;
    if (current->counter > 0 || current->preempt_count > 0){
        return;
    }
    current->counter = 0;
    enable_irq();
    _schedule();
    disable_irq();
}