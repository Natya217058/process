#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>

#define FILENAME "/tmp/module_time_log.txt"

static struct file *file;
static char buffer[100];

static int __init my_module_init(void)
{
    struct timespec ts;
    ts = current_kernel_time();

    file = filp_open(FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (IS_ERR(file)) {
        printk(KERN_ALERT "Failed to open file\n");
        return PTR_ERR(file);
    }

    snprintf(buffer, sizeof(buffer), "Module loaded at: %lld.%09ld\n", ts.tv_sec, ts.tv_nsec);
    kernel_write(file, buffer, strlen(buffer), &file->f_pos);
    return 0;
}

static void __exit my_module_exit(void)
{
    struct timespec ts;
    ts = current_kernel_time();

    snprintf(buffer, sizeof(buffer), "Module unloaded at: %lld.%09ld\n", ts.tv_sec, ts.tv_nsec);
    kernel_write(file, buffer, strlen(buffer), &file->f_pos);
    filp_close(file, NULL);
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A module that logs load/unload times.");
