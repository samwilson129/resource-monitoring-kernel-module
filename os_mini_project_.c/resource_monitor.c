#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h> 
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>

#define TASK_COMM_LEN 16
unsigned long total_memory_utilized = 0;
struct process_node {
    int pid;
    char comm[TASK_COMM_LEN];
    unsigned long rss;
    unsigned long cpu_usage;
    int priority;
    struct process_node *left, *right;
};

struct process_node* newProcessNode(struct task_struct *task) {
    unsigned long total_time = 0;
    unsigned long process_time;
    struct task_struct *p_task;
    struct process_node* node;

    for_each_process(p_task) {
        total_time += p_task->utime + p_task->stime;
    }

    process_time = task->utime + task->stime;

    node = kmalloc(sizeof(struct process_node), GFP_KERNEL);
    if (!node) {
        printk(KERN_ERR "Failed to allocate memory for process node\n");
        return NULL;
    }

    node->pid = task->pid;
    strncpy(node->comm, task->comm, TASK_COMM_LEN);
    node->rss = get_mm_rss(task->mm) * PAGE_SIZE / 1024;
    if (total_time == 0) {
        node->cpu_usage = 0; 
    } else {
        node->cpu_usage = (process_time * 100) / total_time;
    }
    node->priority = task->prio;
    node->left = node->right = NULL;
    total_memory_utilized += node->rss;
    return node;
}

struct process_node* insertProcess(struct process_node* root, struct task_struct *task) {
    if (root == NULL)
        return newProcessNode(task);

    if (task->pid < root->pid)
        root->left = insertProcess(root->left, task);
    else if (task->pid > root->pid)
        root->right = insertProcess(root->right, task);

    return root;
}

const char* priorityToString(int priority) {
    if (priority < 60)
        return "high";
    else if (priority < 100)
        return "mid";
    else
        return "low";
}

void inOrder(struct process_node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printk(KERN_INFO "PID:%-6d |  Name:%-20s |  Memory Usage (RSS):%-6lu KB |  CPU Usage:%-3lu%% |  Priority:%s |\n",
               root->pid, root->comm, root->rss, root->cpu_usage, priorityToString(root->priority));
        inOrder(root->right);
    }
}

static int __init memory_cpu_info_init(void) {
    struct task_struct *task;
    struct process_node *root = NULL;

    printk(KERN_INFO "Memory, CPU usage, and Priority for each process:\n");
    for_each_process(task) {
        if (task->mm) {
            root = insertProcess(root, task);
        }
    }
    inOrder(root);
    printk(KERN_INFO "Total Memory Utilized: %lu KB\n", total_memory_utilized);
    return 0;
}

static void __exit memory_cpu_info_exit(void) {
    printk(KERN_INFO "Memory, CPU info module removed.\n");
}

module_init(memory_cpu_info_init);
module_exit(memory_cpu_info_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanket");
MODULE_DESCRIPTION("A simple kernel module to display memory, CPU usage, and priority for all processes using a binary tree");

