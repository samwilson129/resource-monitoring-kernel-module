#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h> 
#include <linux/mm.h>
#include <linux/sched/signal.h>

static int __init memory_cpu_info_init(void) {
    struct task_struct *task;
    unsigned long total_memory = 0;
    unsigned long total_time = jiffies;
    printk(KERN_INFO "Memory and CPU usage for each process:\n");
    for_each_process(task) {
        struct mm_struct *mm = task->mm;
        if (mm) {
            unsigned long rss = get_mm_rss(mm);
            unsigned long utime = task->utime;
            unsigned long stime = task->stime;
            unsigned long process_time = utime + stime;
            unsigned long cpu_usage = (process_time * 100) / total_time;
            printk(KERN_INFO "PID: %d, Name: %s, Memory Usage (RSS): %ld KB, CPU Usage: %lu%%\n",
                   task->pid, task->comm, rss * PAGE_SIZE / 1024, cpu_usage);
            total_memory += rss;
        }
    }
    printk(KERN_INFO "Total memory in use by all processes: %lu KB\n", total_memory * PAGE_SIZE / 1024);
    return 0;
}

static void __exit memory_cpu_info_exit(void) {
    printk(KERN_INFO "Memory and CPU info module removed.\n");
}

module_init(memory_cpu_info_init);
module_exit(memory_cpu_info_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanket");
MODULE_DESCRIPTION("A simple kernel module to display memory and CPU information for all processes");

