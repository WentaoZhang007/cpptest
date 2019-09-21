// 在Linux下的中断方式读取按键驱动程序
//包含外部中断 休眠 加入poll机制
// 采用异步通知的方式
// 驱动程序发 ---> app接收 （通过kill_fasync()发送）
// 为了使设备支持异步通知机制，驱动程序中涉及以下3项工作：
// 1. 支持F_SETOWN命令，能在这个控制命令处理中设置filp->f_owner为对应进程ID。
// 不过此项工作已由内核完成，设备驱动无须处理。
// 2. 支持F_SETFL命令的处理，每当FASYNC标志改变时，驱动程序中的fasync()函数将得以执行。
// 驱动中应该实现fasync()函数。
// 3. 在设备资源可获得时，调用kill_fasync()函数激发相应的信号
// 应用程序：
// fcntl(fd, F_SETOWN, getpid()); // 告诉内核，发给谁
// Oflags = fcntl(fd, F_GETFL);
// fcntl(fd, F_SETFL, Oflags | FASYNC); // 改变fasync标记，最终会调用到驱动的faync > fasync_helper：初始化/释放fasync_struct
// 外部中断测试程序 包含poll机制 进程之间异步通信 加入原子操作
// 原子操作:指的是在执行过程中不会被别的代码路径所中断的操作。
// 信号量的实现
// 阻塞 :是指在执行设备操作时若不能获得资源则挂起进程，直到满足可操作的条件后再进行操作。
// 被挂起的进程进入休眠状态，被从调度器的运行队列移走，直到等待的条件被满足。
// 非阻塞:进程在不能进行设备操作时并不挂起，它或者放弃，或者不停地查询，直至可以进行操作为止。
// 加入定时器消抖动功能

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>
#define usingatomic (0) // 0使用信号量 1使用的是原子操作

//设备类
static struct class *Eint_class;

// 设备节点
static struct class_device *Eint_class_devs;

// 地址映射
volatile unsigned long *gpfcon;
volatile unsigned long *gpfdat;
volatile unsigned long *gpgcon;
volatile unsigned long *gpgdat;

// 全局变量 存放中断读出的键值
static unsigned int key_val;

//创建一个休眠队列
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

/* 中断事件标志, 中断服务程序将它置1，third_drv_read将它清0 */
static volatile int ev_press = 0;

//信号量初始化结构体
static struct fasync_struct *button_async_queue;
// 定时器结构体
struct timer_list buttons_timer;

// 存储外部中断号和键值结构体变量
static struct pin_desc *irq_pd;

//定义结构体 存放按键 pin 端口 key_val键值
struct pin_desc
{
unsigned int pin;
unsigned int key_val;
};

#if usingatomic
//定义原子变量v并初始化为1
    atomic_t canopen = ATOMIC_INIT(1);
#else
    //定义互斥锁 信号量
    static DECLARE_MUTEX(button_lock);
#endif

//定义结构体数组 存放中断端口和键值
struct pin_desc pins_desc[4]={ 
    {S3C2410_GPF0,0x01},
    {S3C2410_GPF2,0x02},
    {S3C2410_GPG3,0x03},
    {S3C2410_GPG11,0x04}
};

//中断服务程序
//读取键值
static irqreturn_t buttons_irq(int irq, void *ignored)
{
    irq_pd = ( struct pin_desc *)ignored;
    mod_timer(&buttons_timer, jiffies+HZ/100); //10ms 产生中断
    // return IRQ_RETVAL(IRQ_HANDLED);
    return IRQ_HANDLED;
}

//定时器中断函数
static void buttons_timer_function(unsigned long data)
{
    struct pin_desc *pins_desc= irq_pd;
    unsigned int pinval;
    if(!pins_desc)
        return;
    pinval=s3c2410_gpio_getpin(pins_desc->pin); //读取IO的值
    if(pinval)
    {
        key_val =0x80|pins_desc->key_val;
    }
    else
    {
        key_val =pins_desc->key_val;
    }
    ev_press = 1; /* 表示中断发生了 */
    wake_up_interruptible(&button_waitq);  /* 唤醒休眠的进程 */
    kill_fasync (&button_async_queue, SIGIO, POLL_IN); //发送信号给app
}

//打开设备调用,//初始化IO端口 配置为输入模式,//GPF0-->S2 GPF2-->S3 GPG3-->S4 GPG11-->S5
static int Eint_drv_open(struct inode *inode, struct file *file)
{
// *gpfcon &=~((3<<2*0)|(3<<2*2));
// *gpgcon &=~((3<<3*2)|(3<<11*2));
#if usingatomic
    if(!atomic_dec_and_test(&canopen))// 原子操作
    {
        atomic_inc(&canopen);//自加1
        printk("this a user in the use of\n");
        return -EBUSY;//返回忙
    }
#else
    if (file->f_flags & O_NONBLOCK)
    {
//非阻塞 立马返回
        if (down_trylock(&button_lock))
            return -EBUSY;
    }
    else
    {
        down(&button_lock);
    }
#endif
    printk("Eint_drv_open successed!\n");
    request_irq(IRQ_EINT0,buttons_irq, IRQT_BOTHEDGE, "s2", &pins_desc[0]);//EINT0边沿触发方式
    request_irq(IRQ_EINT2,buttons_irq, IRQT_BOTHEDGE, "s3", &pins_desc[1]);//EINT2边沿触发方式
    request_irq(IRQ_EINT11,buttons_irq, IRQT_BOTHEDGE, "s4", &pins_desc[2]);//EINT11边沿触发方式
    request_irq(IRQ_EINT19,buttons_irq, IRQT_BOTHEDGE, "s5", &pins_desc[3]);//EINT19边沿触发方式
    return 0;
}

//write时候调用
static ssize_t Eint_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
}

//read时候调用
//读取按键值
ssize_t Eint_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
    if(size != 1)
        return -EINVAL;
    if (file->f_flags & O_NONBLOCK)
    {
        //非阻塞 立马返回
        if (!ev_press)
        return -EAGAIN;
    }
    else
    {
    //如果没有按键按下 则休眠
        wait_event_interruptible(button_waitq,ev_press);
    }
    //如果有按键动作发生的话 则返回
    copy_to_user(buf, &key_val, 1);
    ev_press = 0;//清中断标志位
    // printk("key_val = 0x%x\n", key_val);
}

//关闭驱动时候调用
static int Eint_drv_colse(struct inode *inode, struct file *file)
{
#if usingatomic
    atomic_inc(&canopen);//自加1
#else
    up(&button_lock);   
#endif

    free_irq(IRQ_EINT0, &pins_desc[0]);//EINT0释放中断
    free_irq(IRQ_EINT2, &pins_desc[1]);//EINT2释放中断
    free_irq(IRQ_EINT11, &pins_desc[2]);//EINT11释放中断
    free_irq(IRQ_EINT19, &pins_desc[3]);//EINT19释放中断
    printk("Eint_drv_colse successed!\n");

}

//poll时候调用
// 在规定时间内没有按下按键 就返回超时
//中断没有发生 就return 0，在do_sys_poll中就会让系统休眠,唤醒休眠是chedule_timeout(__timeou)超时
//中断发生 return POLLIN | POLLRDNORM，在do_sys_poll退出休眠，唤醒进程

static unsigned int Eint_drv_poll(struct file *file, poll_table *wait)
{
    unsigned int mask = 0;
    poll_wait(file, &button_waitq, wait); //加入队列 不会立即休眠
    if (ev_press)
        mask |= POLLIN | POLLRDNORM;
    return mask;
}

//在应用程序中使用fcnt() 时候调用
static int Eint_drvl_fasync (int fd, struct file *filp, int on)
{
    printk("\ndrivec:signal_fasync successed !\n");
    return fasync_helper (fd, filp, on, &button_async_queue);
}

//告诉内核
static struct file_operations Eint_drv_fops = {
    .owner = THIS_MODULE, // 这是一个宏，推向编译模块时自动创建的__this_module变量
    .open = Eint_drv_open,
    .write = Eint_drv_write,
    .read = Eint_drv_read,
    .release= Eint_drv_colse,
    .poll = Eint_drv_poll,
    .fasync = Eint_drvl_fasync,
};

int major;//自动分配主设备号

//安装驱动的时候调用
//注册驱动 创建设备类 创建设备节点 创建虚拟地址 创建定时器任务
int Eint_drv_init(void)
{
// 创建一个定时器
    init_timer(&buttons_timer);
    buttons_timer.function = buttons_timer_function;
    add_timer(&buttons_timer);
    major=register_chrdev( 0, "key_drv",&Eint_drv_fops);//告诉内核 注册驱动
    Eint_class = class_create(THIS_MODULE, "key_drv");//获取一个设备信息类
    if (IS_ERR(Eint_class))
        return PTR_ERR(Eint_class);
    Eint_class_devs = class_device_create(Eint_class, NULL, MKDEV(major, 0), NULL, "buttons");
    if (unlikely(IS_ERR(Eint_class_devs)))
        return PTR_ERR(Eint_class_devs);

//转换虚拟地址
    gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
    gpfdat = gpfcon+1;
    gpgcon =(volatile unsigned long *)ioremap(0x56000060,16);
    gpgdat =gpgcon+1;
    printk("Eint_drv_init successed!\n");
    return 0;

}

//卸载驱动程序的时候调用
//卸载驱动 删除设备 删除设备节点 删除地址映射

void Eint_drv_exit(void)
{
    unregister_chrdev( major, "key_drv");//卸载驱动
    class_device_unregister(Eint_class_devs);//删除设备
    class_destroy(Eint_class);//删除设备节点
    iounmap(gpgcon);//删除地址映射
    iounmap(gpfcon);
    printk("\nEint_drv_exit successed!\n");
}

module_init(Eint_drv_init);
module_exit(Eint_drv_exit);
MODULE_LICENSE("GPL");

//本篇文章来源于 Linux公社网站(www.linuxidc.com)  原文链接：https://www.linuxidc.com/Linux/2016-12/138154.htm