#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 int main(void) {
     int data_processed; //用于记录已经处理的数据
     int file_pipes[2];  //定义两个文件描述符
	 
     char buffer[BUFSIZ + 1];        //定义一个缓冲区，以便从管道中读取数据
     const char * msg = "lwy"; //定义一个将要写入管道中的字符串
     memset(buffer, '\0', sizeof(buffer)); //初始化读取缓冲区
     int fork_result;
	 
     if (pipe(file_pipes) == 0) {          //创建管道
         fork_result=fork();    	  	//创建子进程
         if(fork_result==-1){
             fprintf(stderr,"创建子进程失败");
             exit(EXIT_FAILURE);
         }
         if(fork_result==0){  //说明在子进程中
             data_processed = read(file_pipes[0], buffer, BUFSIZ);  //子进程从缓冲区读入
             printf("子進程读出%d个字符。内容是：%s\n",data_processed,buffer);
         }else{               //说明在父进程中
             data_processed = write(file_pipes[1], msg, sizeof(msg));  //父进程写入缓冲区
             printf("父進程写入%d个字符\n", data_processed);
			 printf("\n");
         }
         exit(EXIT_SUCCESS);
     }
     return EXIT_SUCCESS;
 }
 
 

#include <signal.h>  
#include <stdio.h>  
#include <unistd.h>  
  
void ouch(int sig)  
{  
    printf("\nM! - I got signal %d\n", sig);  
    //恢复终端中断信号SIGINT的默认行为  
    (void) signal(SIGINT, SIG_DFL);  
}  
  
int main()  
{  
    //改变终端中断信号SIGINT的默认行为，使之执行ouch函数  
    //而不是终止程序的执行  
    (void) signal(SIGINT, ouch);  
    while(1)  
    {  
        printf("Hello N!\n");  
        sleep(1);  
    }  
    return 0;  
}
#i#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
int main() {
    void *shmaddr = NULL;
    const char data[] = "Hello World\n";
    int shmid;
    key_t key = (key_t) 666;
    long page_size = sysconf(_SC_PAGESIZE);
    int data_size =  (strlen(data) + page_size - 1) & (~(page_size - 1));
    printf("data size: %d, page size: %ld\n", data_size, page_size);
 
    // 1. create shared memory
    shmid = shmget(key, data_size, 0644 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed\n");
        exit(EXIT_FAILURE);
    }
 
    // 2. attach shared memory
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1) {
        perror("shmat failed\n");
        exit(EXIT_FAILURE);
    }
 
    // 3. read data to shared memory
    printf("read form shead memory: %s\n", (char *)shmaddr);
 
    // 4. detach shared memory
    if (shmdt(shmaddr) == -1) {
        perror("shmdt failed\n");
        exit(EXIT_FAILURE);
    }
 
    // 5. delete shared memory
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl delete shared memory failed\n");
        exit(EXIT_FAILURE);
    }
 
    return 0;
}


asmlinkage int sys_lwy(void)
{
        printk("liang wei ye 20173708036");
        return 1;
}

#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
        syscall(335);
        return 0;
}
