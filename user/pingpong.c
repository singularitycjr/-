
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pipe_ptoc[2];
    int pipe_ctop[2];
    char buf[5] = {'\0'}; // 传递内容为ping 或 pong

    int state1 = pipe(pipe_ptoc);
    int state2 = pipe(pipe_ctop);

    if (state1 == -1 || state2 == -1)
    {
        printf("pipe create failed");
        exit(-1);
    }
    int ret = fork();

    if (ret == -1)
    {
        // 出现错误
        printf("fork failed");
        exit(-1);
    }
    else if (ret == 0)
    {

        // 子进程
        int child_pid = getpid();
        close(pipe_ptoc[1]);
        close(pipe_ctop[0]);
        read(pipe_ptoc[0], buf, 4);     // 读取父进程传递的内容ping
        //注意，这句打印一定要在write之前，此时父进程仍被阻塞，如果在write之后打印则会两个打印交替进行
        printf("%d: received %s\n", child_pid, buf);

        write(pipe_ctop[1], "pong", 4); // 写入pong
        //printf("%d: received %s\n", child_pid, buf);
        exit(0);
    }
    else
    {
        // 父进程
        int parent_pid = getpid();
        close(pipe_ctop[1]);
        close(pipe_ptoc[0]);
        write(pipe_ptoc[1], "ping", 4); // 写入ping
        read(pipe_ctop[0], buf, 4);     // 读取子进程传递的内容pong
        printf("%d: received %s\n", parent_pid, buf);

        wait(0);// 等待子进程结束，传子进程pid，即0
        exit(0);
    }
}
