#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>

/*GNU public license*/
MODULE_LICENSE("GPL");

/*struct contains expires, func, data of timer*/
struct timer_list my_timer;

void
timer_callback (struct timer_list *timer)
{
	/*kernel module use pr_info instead of printf etc..*/
	pr_info("Timer callback: Logging every 5 second\n");
	
	/*re register timer*/
	mod_timer(timer, jiffies+msecs_to_jiffies(5000));
}

int __init 
init_module (void)
{
	pr_info("Timer module initialized\n");
	
	/*initialize timer*/
	timer_setup (&my_timer, timer_callback,0);
	
	/*register timer to kernel
	 *run evrey 5seconds
	 * */
	mod_timer(&my_timer, jiffies+msecs_to_jiffies(5000));

	return 0;
}

void __exit
cleanup_module (void)
{	
	/*delete timer*/
	del_timer(&my_timer);
	pr_info("timer cleanup\n");

	return;
}

/*
 * vim:ai:bs=2:sts=2:sw=2:expandtab:nowrap
 */ 

