/* Author : Teja Sasank Gorthi
   Email  : jet.sasank@gmail.com */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/tty.h>
#include <linux/sched/prio.h>
#include <linux/mm.h>

struct task_struct *task;
struct cred *task_cred;
struct signal_struct *signal;
struct tty_struct *process_terminal;
struct passwd *pw;

int list_task_init(void)
{
	char *tty_name = "?";
	char *pstate = "S";
	int count = 0;
	unsigned long rss;

	printk(KERN_INFO "Loading Module that lists the tasks\n");
	printk(KERN_INFO "UID\tPID\tPPID\tS\tPRI\tNI\tTTY\tRSS\tCMD");
	printk(KERN_INFO "--------------------------------------------------------------------------------");

	for_each_process(task) {

		int nice_value = task_nice(task);
		int priority = task->prio - NICE_WIDTH;
		long state = task->state;

		task_cred = task->cred;
		signal = task->signal;
		process_terminal = signal->tty;

		count++;
		if (process_terminal != NULL) {
			tty_name = process_terminal->name;
		}

		switch(state) {
			case 0:
				pstate = "R";
				break;
			case 1:
				pstate = "S";
				break;
			case 2:
				pstate = "D";
				break;
			case 4:
				pstate = "T";
				break;
			case 8:
				pstate = "Z";
				break;
			case 64:
				pstate = "X";
				break;
			default:
				break;
		}
if (task->mm) {
		rss = get_mm_rss(task->mm);}
		printk(KERN_INFO "%i\t%i\t%i\t%s\t%i\t%i\t%s\t%ld\t%s", 
			task_cred->uid,task->pid,task->parent->pid,pstate, priority ,nice_value,tty_name,rss,task->comm);

	}
printk(KERN_INFO "Number of Processes: %i", count);
	return 0;
}


void list_task_exit(void) {
	printk(KERN_INFO "Removing Module that lists the tasks\n");
}

/* Macros for registering module entry and exit points. */
module_init( list_task_init );
module_exit( list_task_exit );

MODULE_DESCRIPTION("Module that list the tasks");
MODULE_AUTHOR("Teja Sasank Gorthi");
/* By declaring the code license as GPL we can get rid of taint*/
MODULE_LICENSE("GPL");
