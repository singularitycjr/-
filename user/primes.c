#include "kernel/types.h"
#include "user/user.h"

#define READ_END 0//管道读端
#define WRITE_END 1//管道写端
#define MAX_VALUE 35//最大值


void primeSearch(int read_fd)
{
    int prime;
    int p_pipe[2];
    pipe(p_pipe);
    int ret = read(read_fd,&prime,sizeof(int));//读取管道中的第一个数作为当前进程的质数
    if(ret == 0)//如果读取失败，说明已经没有数了，退出
    {
        close(p_pipe[WRITE_END]);
        close(p_pipe[READ_END]);
        exit(0);
    }
    printf("prime %d\n",prime);
    int pid = fork();
    if(pid == 0)
    {
        close(p_pipe[WRITE_END]);
        close(read_fd);
        //为子进程则递归调用
        primeSearch(p_pipe[READ_END]);
    }
    else
    {
        close(p_pipe[READ_END]);
        int num;
        //逐个读取，如果不是当前质数的倍数则写入管道（这些数已经被前面管道中的素数筛过了）
        while(read(read_fd,&num,sizeof(int)))
        {
            if(num % prime != 0)
            {
                write(p_pipe[WRITE_END],&num,sizeof(int));
            }
        }
        close(read_fd);
        close(p_pipe[WRITE_END]);
        wait(0);
        exit(0);
    }
}
int main(int argc, char *argv[])
{
    int p_pipe[2];
    pipe(p_pipe);
    for(int i=2;i<=MAX_VALUE;i++)
    {
        write(p_pipe[1],&i,sizeof(int));
    }
    close(p_pipe[WRITE_END]);
    primeSearch(p_pipe[READ_END]);
    exit(0);
}



