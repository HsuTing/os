#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sched.h>

char *msg = "";

static int proc_show(struct seq_file *file, void *v) {
   struct task_struct *pl;

   for_each_process(pl)
      seq_printf(file, "%s %d\n", pl->comm, pl->pid);

   return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
   return single_open(file, proc_show, NULL);
}

static const struct file_operations proc_fops = {
   .owner = THIS_MODULE,
   .open = proc_open,
   .read = seq_read,
   .llseek = seq_lseek,
   .release = single_release,
};

static int __init init_func(void){
   proc_create(msg, 0, NULL, &proc_fops);
   return 0;
}
static void __exit cleanup_func(void){
   printk("<0>""goodbye\n");
   remove_proc_entry(msg, NULL);
}

module_param(msg, charp, S_IRUGO);
module_init(init_func);
module_exit(cleanup_func);
