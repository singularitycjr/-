#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int num;

char* basename(char *path){
    char *p = path+strlen(path)-1;
    char *q = path;
    while(*p != '\0'){
        if(*p == '/'){
            q = p + 1;
            break;
        }
        p--;
    }
    return q;
}

void find(char* current_path,char* target)
{
    char buf[512], *p;
    int fd;//存储文件描述符的值
    struct dirent de;//存储目录项
    struct stat st;//存储元数据
    if((fd = open(current_path, O_RDONLY)) < 0){
        printf( "find: cannot open %s\n", current_path);
        return;
    }
    if(fstat(fd, &st) < 0){
        printf( "find: cannot stat %s\n", current_path);
        close(fd);//记得关闭文件
        return;
    }
    switch(st.type){
        //如果是文件，判断是否是目标文件
        case T_FILE:
            //printf("%s\n",basename(current_path));
            if(strcmp(basename(current_path),target)==0)
            {
                printf("%s\n",current_path);
                num+=1;
            }
            break;
        //如果是目录，递归调用find
        case T_DIR:
            if(strlen(current_path) + 1 + DIRSIZ + 1 > sizeof (buf)){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, current_path);
            p = buf+strlen(buf);
            *p++ = '/';
            //打开文件夹并递归调用find
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                //de.inum表示节点号，如果为0则表示该目录项为空
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0';//在buf末尾添加结束符
                //不许递归到当前目录或父目录
                if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0)
                {
                    continue;
                }
                find(buf,target);
            }
            break;
    }
    close(fd);//记得关闭文件
}
int main(int argc, char *argv[]){
    if(argc!=3)
    {
        printf("usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    //如果没有找到文件，打印错误信息
    if(num==0)
    {
        printf("find: cannot find file %s\n",argv[2]);
    }
    exit(0);
}