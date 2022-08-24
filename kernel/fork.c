#include <printf.h>
#include <mm.h>
#include <schedule.h>
#include <entry.h>

int copy_process(unsigned long func, unsigned long arg){
    preempt_disable();
    struct task_struct* p;
    p = (struct task_struct*) get_free_page();

    if (!p){
        return 1;
    }
    p->priority = 1;
    p->state = TASK_RUNNING;
    p->counter = p->priority;
    p->preempt_count = 1;

    p->cpu_context.x19 = func;
    p->cpu_context.x20 = arg;
    p->cpu_context.pc = (unsigned long) ret_from_fork;
    p->cpu_context.sp = (unsigned long) p + THREAD_SIZE;

    int pid = nr_tasks++;
    task[pid] = p;
    
	printf("\n\r----------- Task[%d] created -----------\r\n", pid);
	printf("\n\rStruct task allocated at 0x%08x.\r\n", p);
	printf("p->cpu_context.x19 = 0x%08x. (fn)\r\n", p->cpu_context.x19);
	printf("p->cpu_context.x20 = 0x%08x. (arg)\r\n", p->cpu_context.x20);
	printf("p->cpu_context.pc  = 0x%08x. (ret_from_fork)\r\n", p->cpu_context.pc);
	printf("p->cpu_context.sp  = 0x%08x. (sp)\r\n", p->cpu_context.sp);
	printf("p->counter  = %d.\r\n", p->counter);

	preempt_enable();
	return 0;
}