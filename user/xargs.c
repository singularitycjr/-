
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

//#define MAX_BUF_SIZE 512
#define FALSE 0
#define TRUE 1
#define NULL ((void*)0)

int main(int argc, char *argv[]) {

    int arg_num=0;
    char* args[MAXARG];//存储xargs之后的参数，指针数组
    char front_args_buf[MAXARG]={'\0'};//存储xargs之前输出结果（以作为xargs之后续上的参数）
    char* p_buf=front_args_buf;//指向本轮迭代front_args_buf空位置的指针
    char* p_arr=front_args_buf;

    int length=0;
    int buf_used_size=0;

    for (; arg_num+1 < argc; arg_num++) {
        args[arg_num ] = argv[arg_num+1];
        //printf("%d,%s\n",arg_num,args[arg_num]);
    }

    while((length=read(0,p_buf,MAXARG))>0)
    {
        buf_used_size+=length;
        if(buf_used_size>MAXARG)
        {
            //printf("xargs: input too long\n");
            exit(0);
        }

        for(int i = 0; i < length; ++i)
        {
            if(p_buf[i]==' ')
            {
                p_buf[i]='\0';
                //args[arg_num++]=p_buf;
                args[arg_num++]=p_arr;//注意不是p_buf，因为p_buf在一轮迭代中不变
                //p_arr+=i+1; //每次都加了个i，这是错的
                p_arr=p_buf+i+1;
                //printf("%d,%s\n",arg_num-1,args[arg_num-1]);
            }
            else if(p_buf[i]=='\n')
            {
                p_buf[i]='\0';
                //args[arg_num++]=p_buf;
                args[arg_num++]=p_arr;//注意不是p_buf，因为p_buf在一轮迭代中不变
                p_arr=p_buf+i+1;
                args[arg_num]=NULL;//最后一个参数为NULL表示结束
                //printf("%d,%s\n",arg_num-1,args[arg_num-1]);
                int pid=fork();
                if(pid==0)
                {
                    if(exec(argv[1],args)==-1)
                    {
                        printf("xargs: exec failed\n");
                        exit(-1);
                    }
                    exit(0);//退出子进程
                }
                 
                arg_num=argc-1;//重置arg_num，指向第一个空位置
                wait(0);//等待子进程结束
            }
        }
        p_buf+=length;//指向下一轮迭代的空位置
    }
    exit(0);
}
/*
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_BUF_SIZE 512
#define FALSE 0
#define TRUE 1
int main(int argc, char *argv[]) {
    char* args[MAXARG];//存储xargs之后的参数
    char front_args_buf[MAX_BUF_SIZE];//存储xargs之前输出结果（以作为xargs之后续上的参数）
    char* p_buf=front_args_buf;//指向front_args_buf空位置的指针
    int arg_num;
    int _n_flag=FALSE;

    if(strcmp(argv[1],"-n")==0)
    {
        int temp=atoi(argv[2]);
        if(temp!=0 && temp<=MAXARG)
        {
        arg_num=atoi(argv[2]);
        _n_flag=TRUE;
        }
        else
        {
            printf("xargs: invalid number for -n\n");
            exit(0);
        }
    }
    else
    {
        arg_num=MAXARG;
    }

    for (int i = _n_flag?3:1,j=0; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    int length=0;
    int buf_used_size=0;
    while((length=read(0,p_buf,MAX_BUF_SIZE))>0)
    {
        buf_used_size+=length;
    }
}
*/