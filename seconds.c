/** 
 * seconds.c
 *
 * Kernel module that communicates with /proc file system.
 * 
 * The makefile must be modified to compile this program.
 * Change the line "simple.o" to "seconds.o"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

unsigned long int volatile init_jiffies, exit_jiffies;
const int hz=HZ;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count,loff_t *pos);

static struct file_operations proc_ops = {
   .owner = THIS_MODULE,
   .read = proc_read,
};

int proc_init(void)
{
    proc_create(PROC_NAME,0666,NULL,&proc_ops);
    init_jiffies = jiffies;
    printk(KERN_INFO "Loading module seconds.\n");
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME,NULL);
    exit_jiffies = jiffies;
    printk(KERN_INFO "Removing module seconds.\n");
    printk(KERN_INFO "The total running time is %d s\n", ((exit_jiffies-init_jiffies)/hz));
}


ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
      completed = 0;
      return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "The value of jiffies is %lu\n", jiffies);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("SGG");


//================================================================================
// MakeFile for seconds.c
//================================================================================

/*

obj-m += seconds.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

*/



//================================================================================
// Shell script (insrem.sh) for seconds.c
//================================================================================

/*


#!/bin/bash

# clear the message queue, then
# insert seconds.ko into the kernal and remove it again, then
# dispaly the messages as a result of doing so

sudo -p "osc"
sudo dmesg -c
clear

printf "beginning script...\n\n"
sudo insmod seconds.ko
cat /proc/seconds
sleep 3 
cat /proc/seconds
sleep 3 
sudo rmmod seconds
sudo dmesg
printf "\n...end of script\n\n"



*/




//================================================================================
// Output for seconds.c
//================================================================================


/*


beginning script...

The value of jiffies is 4297613278
The value of jiffies is 4297614029
[10884.416881] Loading module seconds.
[10890.427260] Removing module seconds.
[10890.427263] The total running time is 6 s

...end of script


*/