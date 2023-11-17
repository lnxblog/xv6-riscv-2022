#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main()
{

    int pipefd[2];
    int pipefd2[2];
    int num;
    int toggle=0;
    int rd_fd_n,wr_fd_n;
    int rd_fd_p,wr_fd_p;
  

    if(pipe(pipefd) == -1)
    {
        printf("pipe failed\n");
        return 1;
    }

    
    if(fork())
    {
       // printf("process %d\n",getpid());
        close(pipefd[0]);  // close read fd
        int i = 2;
        while(i<=35)
        {
            if(write(pipefd[1],(void*)&i,4) == -1)
            {
                printf("write failed\n");
                return 1;
            }
            i++;
        }
        close(pipefd[1]);
        wait(0);
       // printf("parent exiting\n");
    }
    else
    {
        while(1)
        {
            if(!toggle)
            {
                rd_fd_p = pipefd[0];  // fd to read previous process
                wr_fd_p = pipefd[1];
                pipe(pipefd2);
                wr_fd_n = pipefd2[1];  // fd to write to next process

                rd_fd_n = pipefd2[0];
                close(wr_fd_p);  // important: close your copy of write fd when reading

            }
            else
            {
                rd_fd_p = pipefd2[0];
                wr_fd_p = pipefd2[1];
                pipe(pipefd);
                wr_fd_n = pipefd[1];
                rd_fd_n = pipefd[0];
                close(wr_fd_p);

            }
            toggle = ~toggle;

            int ret = read(rd_fd_p,(void*)&num,4);

            if( ret == 0)  // read prime
            {
                close(rd_fd_p);
                exit(0);
            }
            else if(ret == -1)
            {
                printf("read failed\n");
                exit(1);
            }

            if(fork())  // pipe the numbers to next process
            {
                close(rd_fd_n);
             //   printf("process %d\n",getpid());
                int prime = num;
                printf("prime %d\n",prime);
                while(read(rd_fd_p,(void*)&num,4) != 0)
                {
                  //  printf("received %d\n",num);

                    if(num % prime)
                    {
                        write(wr_fd_n,(void*)&num,4);
                    }

                }
                close(wr_fd_n);wait(0);
                exit(0);
            }
        }
    }

    return 0;
}