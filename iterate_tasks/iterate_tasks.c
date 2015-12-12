/* Author : Teja Sasank Gorthi
   Email  : jet.sasank@gmail.com */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/tty.h>
#include <linux/sched/prio.h>
#include <linux/list.h>
#include <linux/mm.h>


struct task_struct *task;
struct cred *task_cred;
struct signal_struct *signal;
struct tty_struct *process_terminal;
struct passwd *pw;

void iterate_task(struct task_struct *p) {
	struct task_struct *task;
	struct list_head *list;
	


	list_for_each(list, &p->children) {
		task = list_entry(list, struct task_struct, sibling);
		char *tty_name = "?";
		char *pstate = "S";
		unsigned long rss;

		int nice_value = task_nice(task);
		int priority = task->prio - NICE_WIDTH;
		long state = task->state;

		task_cred = task->cred;
		signal = task->signal;
		process_terminal = signal->tty;

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
		if(task != NULL) {
			iterate_task(task);
		}
	}
}
int iterate_tasks_init(void)
{
	printk(KERN_INFO "Loading Module that iterates through the tasks using Depth-First Search Tree\n");
	printk(KERN_INFO "UID\tPID\tPPID\tS\tPRI\tNI\tTTY\tRSS\tCMD");
	printk(KERN_INFO "--------------------------------------------------------------------------------");

	iterate_task(&init_task); /* we are starting the iteration with the init task the parent of every other task in the operating system*/

	return 0;
}


void iterate_tasks_exit(void) {
	printk(KERN_INFO "Removing Module that iterates through the tasks using Depth-First Search Tree\n");
}

/* Macros for registering module entry and exit points. */
module_init( iterate_tasks_init );
module_exit( iterate_tasks_exit );

MODULE_DESCRIPTION("Module that iterates through the tasks using Depth-First Search Tree");
MODULE_AUTHOR("Teja Sasank Gorthi");
/* By declaring the code license as GPL we can get rid of taint*/
MODULE_LICENSE("GPL");
