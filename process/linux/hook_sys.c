/*
 * Hooking kernel functions using ftrace framework
 *
 * Copyright (c) 2018 ilammy
*/

#define pr_fmt(fmt) "ftrace_hook: " fmt

#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/path.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("Example module hooking clone() and execve() via ftrace");
MODULE_AUTHOR("ilammy <a.lozovsky@gmail.com>");
MODULE_LICENSE("GPL");

static int check_signature(pid_t pid);

#define MAX_PROCESS 10
typedef struct ProtectPids{
    int protectPids[MAX_PROCESS];
    int pos;
}ProtectPids;

ProtectPids protectPids = {0};

#define DEVICE_NAME "chardev_example"
#define BUFFER_SIZE 256

static int major_number;
static struct class *chardev_class = NULL;
static struct device *chardev_device = NULL;

static int device_open = 0;

static char kernel_buffer[BUFFER_SIZE];
static int buffer_index = 0;

static int device_open_callback(struct inode *inode, struct file *file) {
    if (device_open)
        return -EBUSY;

    device_open++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release_callback(struct inode *inode, struct file *file) {
    device_open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read_callback(struct file *file, char *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;
    int max_bytes;

    max_bytes = BUFFER_SIZE - *offset;

    if (max_bytes <= 0)
        return 0;

    if (max_bytes > length)
        max_bytes = length;

    if (copy_to_user(buffer, kernel_buffer + *offset, max_bytes))
        return -EFAULT;

    *offset += max_bytes;
    return max_bytes;
}

static ssize_t device_write_callback(struct file *file, const char *buffer, size_t length, loff_t *offset) {
    int pid;
    int ret;

    ret = kstrtoint(buffer, 10, &pid);
    if (ret == 0) {
        printk(KERN_INFO "protect pid is %d\n", pid);
    } else {
        printk(KERN_ERR "kstrtoint failed with error code %d\n", ret);
    }

    if(check_signature(pid) == TRUE){
        protectPids.protectPids[(protectPids.pos)++] = pid;
    }else{
        printk(KERN_ERR "pid :%d, check_signture failed!\n", pid);
    }
    return length;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open_callback,
    .release = device_release_callback,
    .read = device_read_callback,
    .write = device_write_callback,
};

static int message_init(void){
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    chardev_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(chardev_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create a device class\n");
        return PTR_ERR(chardev_class);
    }

    chardev_device = device_create(chardev_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(chardev_device)) {
        class_destroy(chardev_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create a device\n");
        return PTR_ERR(chardev_device);
    }

    printk(KERN_INFO "Chardev Example module loaded with major number %d\n", major_number);
    return 0;

}

static void message_exit(void){
    device_destroy(chardev_class, MKDEV(major_number, 0));
    class_unregister(chardev_class);
    class_destroy(chardev_class);
    unregister_chrdev(major_number, DEVICE_NAME);
}

#define BUF_SIZE 256 

static int get_exe_path(pid_t pid, char* buf)
{
    struct file *file;
    char path[BUF_SIZE];
    struct path *exe_path;
    char *exe_path_str;

    snprintf(path, BUF_SIZE, "/proc/%d/exe", pid);

    file = filp_open(path, O_PATH, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "filp_open failed\n");
        return FALSE;
    }

    exe_path = &file->f_path;

    exe_path_str = d_path(exe_path, buf, BUF_SIZE);
    if (!IS_ERR(exe_path_str)) {
        printk(KERN_INFO "Executable path of PID %d: %s\n", pid, exe_path_str);
    } else {
        printk(KERN_ERR "d_path failed\n");
        return FALSE;
    }

    filp_close(file, NULL);
    return TRUE;
}

static char signature[] = {'g', 'u','o', 'y', 'a', 'w', 'e', 'n'};

static char* signature_file_name = "signature.sig";

static int check_signature(pid_t pid)
{
    char path[BUF_SIZE] = {0};
    char* filename;
    char new_path[BUF_SIZE] = {0};

    struct file *file;
    char buf[BUF_SIZE];
    ssize_t len;


    if(get_exe_path(pid, path) == FALSE){
        printk(KERN_ERR "get_exe_path failed!\n");
        return FALSE;
    }

    filename = strrchr(path, '/');
    if (filename == NULL) {
        printk(KERN_ERR "Invalid path\n");
        return FALSE;
    }

    size_t dir_len = filename - path + 1;  // Include the slash
    strncpy(new_path, path, dir_len);
    new_path[dir_len] = '\0';  // Null-terminate the directory path
    strncat(new_path, "signature.sig", BUF_SIZE - dir_len);
    printk(KERN_INFO "New path: %s\n", new_path);

    // Open the file
    file = filp_open(new_path, O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "filp_open failed\n");
        return FALSE;
    }

    // Read the file
    len = kernel_read(file, buf, BUF_SIZE, 0);
    if (len < 0) {
        printk(KERN_ERR "kernel_read failed\n");
        filp_close(file, NULL);
        return FALSE;
    }

    // Compare the file content with the signature
    // if (len != SIGNATURE_SIZE || memcmp(buf, signature, SIGNATURE_SIZE) != 0) {
    if (memcmp(buf, signature, sizeof(signature)) != 0) {
        printk(KERN_ERR "Signature does not match\n");
        filp_close(file, NULL);
        return FALSE;
    }

    filp_close(file, NULL);
    return TRUE;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)
static unsigned long lookup_name(const char *name)
{
        struct kprobe kp = {
                .symbol_name = name
        };
        unsigned long retval;

        if (register_kprobe(&kp) < 0) return 0;
        retval = (unsigned long) kp.addr;
        unregister_kprobe(&kp);
        return retval;
}
#else
static unsigned long lookup_name(const char *name)
{
        return kallsyms_lookup_name(name);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,11,0)
#define FTRACE_OPS_FL_RECURSION FTRACE_OPS_FL_RECURSION_SAFE
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,11,0)
#define ftrace_regs pt_regs

static __always_inline struct pt_regs *ftrace_get_regs(struct ftrace_regs *fregs)
{
        return fregs;
}
#endif

/*
 * There are two ways of preventing vicious recursive loops when hooking:
 * - detect recusion using function return address (USE_FENTRY_OFFSET = 0)
 * - avoid recusion by jumping over the ftrace call (USE_FENTRY_OFFSET = 1)
 */
#define USE_FENTRY_OFFSET 0

/**
 * struct ftrace_hook - describes a single hook to install
 *
 * @name:     name of the function to hook
 *
 * @function: pointer to the function to execute instead
 *
 * @original: pointer to the location where to save a pointer
 *            to the original function
 *
 * @address:  kernel address of the function entry
 *
 * @ops:      ftrace_ops state for this function hook
 *
 * The user should fill in only &name, &hook, &orig fields.
 * Other fields are considered implementation details.
 */
struct ftrace_hook {
        const char *name;
        void *function;
        void *original;

        unsigned long address;
        struct ftrace_ops ops;
};

static int fh_resolve_hook_address(struct ftrace_hook *hook)
{
        hook->address = lookup_name(hook->name);

        if (!hook->address) {
                pr_debug("unresolved symbol: %s\n", hook->name);
                return -ENOENT;
        }

#if USE_FENTRY_OFFSET
        *((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
#else
        *((unsigned long*) hook->original) = hook->address;
#endif

        return 0;
}

static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
        struct pt_regs *regs = ftrace_get_regs(fregs);
        struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

#if USE_FENTRY_OFFSET
        regs->ip = (unsigned long)hook->function;
#else
        if (!within_module(parent_ip, THIS_MODULE))
                regs->ip = (unsigned long)hook->function;
#endif
}

/**
 * fh_install_hooks() - register and enable a single hook
 * @hook: a hook to install
 *
 * Returns: zero on success, negative error code otherwise.
 */
int fh_install_hook(struct ftrace_hook *hook)
{
        int err;

        err = fh_resolve_hook_address(hook);
        if (err)
                return err;

        /*
         * We're going to modify %rip register so we'll need IPMODIFY flag
         * and SAVE_REGS as its prerequisite. ftrace's anti-recursion guard
         * is useless if we change %rip so disable it with RECURSION.
         * We'll perform our own checks for trace function reentry.
         */
        hook->ops.func = fh_ftrace_thunk;
        hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS
                        | FTRACE_OPS_FL_RECURSION
                        | FTRACE_OPS_FL_IPMODIFY;

        err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
        if (err) {
                pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
                return err;
        }

        err = register_ftrace_function(&hook->ops);
        if (err) {
                pr_debug("register_ftrace_function() failed: %d\n", err);
                ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
                return err;
        }

        return 0;
}

/**
 * fh_remove_hooks() - disable and unregister a single hook
 * @hook: a hook to remove
 */
void fh_remove_hook(struct ftrace_hook *hook)
{
        int err;

        err = unregister_ftrace_function(&hook->ops);
        if (err) {
                pr_debug("unregister_ftrace_function() failed: %d\n", err);
        }

        err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        if (err) {
                pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
        }
}

/**
 * fh_install_hooks() - register and enable multiple hooks
 * @hooks: array of hooks to install
 * @count: number of hooks to install
 *
 * If some hooks fail to install then all hooks will be removed.
 *
 * Returns: zero on success, negative error code otherwise.
 */
int fh_install_hooks(struct ftrace_hook *hooks, size_t count)
{
        int err;
        size_t i;

        for (i = 0; i < count; i++) {
                err = fh_install_hook(&hooks[i]);
                if (err)
                        goto error;
        }

        return 0;

error:
        while (i != 0) {
                fh_remove_hook(&hooks[--i]);
        }

        return err;
}

/**
 * fh_remove_hooks() - disable and unregister multiple hooks
 * @hooks: array of hooks to remove
 * @count: number of hooks to remove
 */
void fh_remove_hooks(struct ftrace_hook *hooks, size_t count)
{
        size_t i;

        for (i = 0; i < count; i++)
                fh_remove_hook(&hooks[i]);
}

#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

/*
 * Tail call optimization can interfere with recursion detection based on
 * return address on the stack. Disable it to avoid machine hangups.
 */
#if !USE_FENTRY_OFFSET
#pragma GCC optimize("-fno-optimize-sibling-calls")
#endif

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_clone)(struct pt_regs *regs);

static asmlinkage long fh_sys_clone(struct pt_regs *regs)
{
        long ret;

        pr_info("clone() before\n");

        ret = real_sys_clone(regs);

        pr_info("clone() after: %ld\n", ret);

        return ret;
}
#else
static asmlinkage long (*real_sys_clone)(unsigned long clone_flags,
                unsigned long newsp, int __user *parent_tidptr,
                int __user *child_tidptr, unsigned long tls);

static asmlinkage long fh_sys_clone(unsigned long clone_flags,
                unsigned long newsp, int __user *parent_tidptr,
                int __user *child_tidptr, unsigned long tls)
{
        long ret;

        pr_info("clone() before\n");

        ret = real_sys_clone(clone_flags, newsp, parent_tidptr,
                child_tidptr, tls);

        pr_info("clone() after: %ld\n", ret);

        return ret;
}
#endif

int pidInProtectPids(int pid)
{
    for(int i = 0; i < MAX_PROCESS; ++i){
        if(pid == protectPids.protectPids[i]){
            return TRUE;
        }
    }
    return FALSE;
}


#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_process_vm_readv)(struct pt_regs *regs);

static asmlinkage long fh_sys_process_vm_readv(struct pt_regs *regs)
{
        long ret;
        int pid = regs->di;

        pr_info("process_vm_readv() before\n");
        if(pidInProtectPids(pid) == TRUE){
            pr_info("process_vm_readv() protected pid %d\n", pid);
            return -EPERM;
        }
        ret = real_sys_process_vm_readv(regs);

        pr_info("process_vm_readv() after: %ld\n", ret);

        return ret;
}
#else
static asmlinkage long (*real_sys_process_vm_readv)(pid_t pid, const void* local_iov,
                       unsigned long liovcnt, const void* remote_iov, unsigned long riovcnt,
                       unsigned long flags);

static asmlinkage long fh_sys_process_vm_readv(pid_t pid, const void* local_iov,
                       unsigned long liovcnt, const void* remote_iov, unsigned long riovcnt,
                       unsigned long flags)
{
        long ret;

        pr_info("process_vm_readv() before\n");

        if(pidInProtectPids(pid) == TRUE){
            pr_info("process_vm_readv() protected pid %d\n", pid);
            return -EPERM;
        }

        ret = real_sys_process_vm_readv(pid, local_iov, liovcnt, remote_iov, riovcnt, flags);

        pr_info("process_vm_readv() after: %ld\n", ret);

        return ret;
}
#endif

static pid_t protect_pid = 5665;

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_ptrace)(struct pt_regs *regs);
static asmlinkage long fh_sys_ptrace(struct pt_regs *regs)
{
        long ret;
        int pid = regs->si;

        pr_info("stub ptrace() before\n");
        if(pidInProtectPids(pid) == TRUE){
            pr_info("process_vm_readv() protected pid %d\n", pid);
            return -EPERM;
        }

        ret = real_sys_ptrace(regs);

        pr_info("stub ptrace() after: %ld\n", ret);

        return ret;
}
#else
static asmlinkage long (*real_sys_ptrace)(unsigned long request, pid_t pid, void* addr, void* data);

static asmlinkage long fh_sys_ptrace(unsigned long request, pid_t pid, void* addr, void* data)
{
        long ret;

        pr_info("ptrace() before\n");
        if(pidInProtectPids(pid) == TRUE){
            pr_info("process_vm_readv() protected pid %d\n", pid);
            return -EPERM;
        }

        ret = real_sys_ptrace(request, pid, addr, data);

        pr_info("ptrace() after: %ld\n", ret);

        return ret;
}
#endif


static char *duplicate_filename(const char __user *filename)
{
        char *kernel_filename;

        kernel_filename = kmalloc(4096, GFP_KERNEL);
        if (!kernel_filename)
                return NULL;

        if (strncpy_from_user(kernel_filename, filename, 4096) < 0) {
                kfree(kernel_filename);
                return NULL;
        }

        return kernel_filename;
}

int delete_pid_inpids(int pid)
{
    for(int i = 0; i < MAX_PROCESS; ++i){
        if(pid == protectPids.protectPids[i]){
            if(i == protectPids.pos){
                protectPids.protectPids[i] = 0;
            }else{
                protectPids.protectPids[i] = protectPids.protectPids[protectPids.pos];
            }
            --protectPids.pos;
        }
    }
}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_do_exit)(struct pt_regs *regs);

static asmlinkage long fh_do_exit(struct pt_regs *regs)
{
        long ret;

        pr_info("exit() before\n");
        struct task_struct *tsk = current;
        pid_t pid = current->pid;
        pr_info("exit pid is %ld", pid);
        pr_info("exit pid is %ld", pid);

        delete_pid_inpids(pid);

        ret = real_do_exit(regs);

        pr_info("exit() after: %ld\n", ret);

        return ret;
}
#else
static asmlinkage long (*real_do_exit)(long errorcode);

static asmlinkage long fh_do_exit(long errorcode)
{
        long ret;

        pr_info("exit() before\n");
        struct task_struct *tsk = current;
        pid_t pid = current->pid;
        pr_info("exit pid is %ld", pid);
        pr_info("exit pid is %ld", pid);

        delete_pid_inpids(pid);

        ret = real_do_exit(errorcode);

        pr_info("exit() after: %ld\n", ret);

        return ret;
}
#endif

/*
 * x86_64 kernels have a special naming convention for syscall entry points in newer kernels.
 * That's what you end up with if an architecture has 3 (three) ABIs for system calls.
 */
#ifdef PTREGS_SYSCALL_STUBS
#define SYSCALL_NAME(name) ("__x64_" name)
#else
#define SYSCALL_NAME(name) (name)
#endif

#define HOOK(_name, _function, _original)       \
        {                                       \
                .name = SYSCALL_NAME(_name),    \
                .function = (_function),        \
                .original = (_original),        \
        }

#define HOOK_PLAIN(_name, _function, _original) \
        {                                       \
                .name     = (_name),    \
                .function = (_function),        \
                .original = (_original),        \
        }

static struct ftrace_hook demo_hooks[] = {
        //HOOK("sys_execve", fh_sys_execve, &real_sys_execve),
        HOOK("sys_process_vm_readv", fh_sys_process_vm_readv, &real_sys_process_vm_readv),
        HOOK("sys_ptrace", fh_sys_ptrace, &real_sys_ptrace),
        HOOK_PLAIN("do_exit", fh_do_exit, &real_do_exit),

};

static int fh_init(void)
{
        int err;

        err = fh_install_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));
        if (err)
                return err;

        pr_info("ftrace_hook module loaded\n");

        return 0;
}
module_init(fh_init);

static void fh_exit(void)
{
        fh_remove_hooks(demo_hooks, ARRAY_SIZE(demo_hooks));

        pr_info("ftrace_hook module unloaded\n");
}
module_exit(fh_exit);
