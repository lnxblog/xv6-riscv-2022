#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char **argv)
{
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        printf("pipe failed\n");
        return 1;
    }

    if(fork()==0)
    {
        char byte;
        if(read(pipefd[0],(void*)&byte,1)==-1)
        {
            printf("read failed\n");
            return 1;
        }

        printf("%d: received ping\n",getpid());

        byte = 0xA;
        if(write(pipefd[1],(void*)&byte,1) == -1)
        {
            printf("write failed\n");
            return 1;
        }

        close(pipefd[0]);
        close(pipefd[1]);
    }
    else
    {
        char byte;

        byte = 0xA;
        if(write(pipefd[1],(void*)&byte,1) == -1)
        {
            printf("write failed\n");
            return 1;
        }

        if(read(pipefd[0],(void*)&byte,1)==-1)
        {
            printf("read failed\n");
            return 1;
        }

        printf("%d: received pong\n",getpid());

        close(pipefd[0]);
        close(pipefd[1]);
    }

    return 0;
}