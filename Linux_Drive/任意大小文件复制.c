#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void file_copy(char *srcFile,char *desFile);

int main(int argc,char* argv[]){
	
	
	file_copy(argv[1],argv[2]);
	
	return 0;
}
//可以复制任意大小的文件 
void file_copy(char *srcFile,char *desFile){
	
	int i = 0;
	char *buff;
	unsigned int fsize = 0;
	FILE *fp_before_cp;
	FILE *fp_after_cp;
	
	fp_before_cp = fopen(srcFile,"r+");  			//打开已有文件
	if(NULL == fp_before_cp){						//没有文件 退出进程
		perror("open file ");
		exit(0);
	}
	while(getc(fp_before_cp) != EOF)				//计算文件里面的字符个数 此时指针偏移值末尾
		fsize++;
		
	buff = (char *)malloc(fsize * sizeof(char));	//为缓冲区分配内存	
	if(NULL == buff){
		perror("malloc 	");
		exit(0);
	}
	rewind(fp_before_cp);							//此时文件指针已经偏移到文件尾 要复位
	//fgets(buff,fsize+1,fp_before_cp);				//读取文本内容到缓冲区(这个不行 遇到第一个换行后面内容全部消失)
	for(i=0;i<fsize;i++){							//读取文本内容到缓冲区
		*(buff+i) = getc(fp_before_cp);
	}
	
	fclose(fp_before_cp);							//关闭源文件
	
	printf("file size : %d bytes\n",fsize);
	printf("file content : \n%s\n",buff);
	
	fp_after_cp = fopen(desFile,"w+");				//创建一个目标文件 把缓冲区内容复制到该文件
	if(NULL == fp_after_cp){
		perror("open file ");
		exit(0);
	}
	fputs(buff,fp_after_cp);
	fclose(fp_after_cp);							//关闭文件
	
	free(buff);										//释放缓冲区
	buff = NULL;
}