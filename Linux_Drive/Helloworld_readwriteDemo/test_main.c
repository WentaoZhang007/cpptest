
#include <sys/types.h>  

#include <unistd.h>  

#include <fcntl.h>  

#include <linux/rtc.h>  

#include <sys/ioctl.h> 

#include <stdio.h>  

#include <stdlib.h>  

#include <sys/stat.h>

 

#define COMMAND1 1  

#define COMMAND2 2  

  

int main(int argc, char **argv)  

{  

    int fd;  

    int i;  

    char data[256] = {0};  

    int retval;  

    char *name = "/dev/sunny";

 

    if(argc == 2)

    {

        name = argv[1];

    }

 

    fd = open(name, O_RDWR | O_CREAT);  

    if (fd == -1) {  

        perror("open error\n");  

        exit(-1);  

    }  

    printf("open %s successfully\n", name);  

 

    retval = write(fd, "yangjin", 7);  

    if (retval == -1) {  

        perror("write error\n");

        close(fd);

        exit(-1);  

    }  

    retval = lseek(fd, 0, 0);  

    if (retval == -1) {  

        perror("lseek error\n");

        close(fd);

        exit(-1);  

    }  

    retval = read(fd, data, 10);  

    if (retval == -1) {  

        perror("read error\n");

        close(fd);

        exit(-1);  

    }  

    printf("read successfully: %s\n", data);

 

    retval = ioctl(fd, COMMAND1, 0);  

    if (retval == -1) {  

        perror("ioctl error\n");  

        close(fd);        

        exit(-1);  

    }  

    printf("ioctl command 1 successfully\n");  

  

    close(fd);  

    return 0;  

}

 
