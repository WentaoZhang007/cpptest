#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>   
#include <asm/irq.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/dma-mapping.h>

#define  S3C_DMA_SIZE   512*1024          //DMA传输长度   512KB

#define NORMAL_COPY     0                 //两个地址之间的正常拷贝
#define DMA_COPY        1                 //两个地址之间的DMA拷贝

/*函数声明*/
static DECLARE_WAIT_QUEUE_HEAD(s3c_dma_queue);          //声明等待队列
static int s3c_dma_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long flags);

  /*
   * 定义中断事件标志
   * 0:进入等待队列        1:退出等待队列
   */
     static int s3c_dma_even=0;


static unsigned char   *source_virt;            //源虚拟地址
static unsigned int     source_phys;            //源物理地址

static unsigned char *dest_virt;              //目的虚拟地址
static unsigned int   dest_phys;              //目的虚拟地址


/*DMA3寄存器*/
struct  S3c_dma3_regs{
    unsigned int disrc3    ;          //0x4b0000c0
    unsigned int disrcc3   ;                    
    unsigned int didst3    ;                    
    unsigned int didstc3   ;               
    unsigned int dcon3     ;                
    unsigned int dstat3    ; 
    unsigned int dcsrc3    ; 
    unsigned int dcdst3    ;        
    unsigned int dmasktrig3;        //0x4b0000e0
};


 static volatile struct S3c_dma3_regs   *s3c_dma3_regs;

/*字符设备操作*/
static struct file_operations  s3c_dma_fops={
        .owner  = THIS_MODULE,
        .ioctl     = s3c_dma_ioctl,
};

/*中断服务函数*/
static irqreturn_t  s3c_dma_irq (int irq, void *dev_id)   
{
    s3c_dma_even=1;                             //退出等待队列
    wake_up_interruptible(&s3c_dma_queue);      //唤醒 中断
    return IRQ_HANDLED;
}

/*ioctl函数*/
static int s3c_dma_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long flags)
{
    int i;
    memset(source_virt, 0xAA, S3C_DMA_SIZE);          
    memset(dest_virt, 0x55, S3C_DMA_SIZE);   
    
    switch(cmd)
    {
    case NORMAL_COPY:                           //正常拷贝
            
             for(i=0;i<S3C_DMA_SIZE;i++)
                 dest_virt[i] =  source_virt[i];

             if(memcmp(dest_virt, source_virt, S3C_DMA_SIZE)==0)
           {
         printk("NORMAL_COPY OK\n");
                return 0;
         }
         else
        {
         printk("NORMAL_COPY ERROR\n");
               return -EAGAIN;
        }             
            
    case DMA_COPY:                               //DMA拷贝

        s3c_dma_even=0;     //进入等待队列
        
        /*设置DMA寄存器,启动一次DMA传输 */
        /* 源的物理地址 */
        s3c_dma3_regs->disrc3      = source_phys;      
        /* 源位于AHB总线, 源地址递增 */  
        s3c_dma3_regs->disrcc3     = (0<<1) | (0<<0);
        /* 目的的物理地址 */
        s3c_dma3_regs->didst3      = dest_phys;      
        /* 目的位于AHB总线, 目的地址递增 */
        s3c_dma3_regs->didstc3     = (0<<2) | (0<<1) | (0<<0);     
        /* 使能中断,单个传输,软件触发, */
        s3c_dma3_regs->dcon3=(1<<30)|(1<<29)|(0<<28)|(1<<27)|(0<<23)|(0<<20)|(S3C_DMA_SIZE<<0);  
        //启动一次DMA传输
        s3c_dma3_regs->dmasktrig3  = (1<<1) | (1<<0);     
        
        wait_event_interruptible(s3c_dma_queue, s3c_dma_even);    //进入睡眠,等待DMA传输中断到来才退出
        
        if(memcmp(dest_virt, source_virt, S3C_DMA_SIZE)==0)
        {
         printk("DMA_COPY OK\n");
                return 0;
         }
        else
        {
       printk("DMA_COPY ERROR\n");
             return -EAGAIN;
           }  

            break;
    }
    return 0;
}


static unsigned int major;
static struct class *cls;
static int s3c_dma_init(void)
{
    /*1.1 注册DMA3 中断  */
    if(request_irq(IRQ_DMA3, s3c_dma_irq,NULL, "s3c_dma",1)) 
    {
        printk("Can't    request_irq   \"IRQ_DMA3\"!!!\n ");
        return -EBUSY;
    }
    
    /*1.2 分配两个DMA缓冲区(源、目的)*/
    source_virt=dma_alloc_writecombine(NULL,S3C_DMA_SIZE, &source_phys, GFP_KERNEL);
    if(source_virt==NULL)       
   {
        printk("Can't  dma_alloc   \n ");
        return -ENOMEM;
   }
    
    dest_virt=dma_alloc_writecombine(NULL,S3C_DMA_SIZE, &dest_phys, GFP_KERNEL);
    if(dest_virt==NULL)       
   {
        printk("Can't  dma_alloc   \n ");
        return -ENOMEM;
   }
    
    
    /*2.注册字符设备,并提供文件操作集合fops*/
    major=register_chrdev(0, "s3c_dma",&s3c_dma_fops);
    cls= class_create(THIS_MODULE, "s3c_dma");
    class_device_create(cls, NULL,MKDEV(major,0), NULL, "s3c_dma");

    s3c_dma3_regs=ioremap(0x4b0000c0, sizeof(struct S3c_dma3_regs));
    
    return 0;  
}

static void s3c_dma_exit(void)
{
    iounmap(s3c_dma3_regs);
    
    class_device_destroy(cls, MKDEV(major,0));
    class_destroy(cls);

    dma_free_writecombine(NULL, S3C_DMA_SIZE, dest_virt, dest_phys);
    dma_free_writecombine(NULL, S3C_DMA_SIZE, source_virt, source_phys);   

    free_irq(IRQ_DMA3, 1);

}
module_init(s3c_dma_init);
module_exit(s3c_dma_exit);
MODULE_LICENSE("GPL");