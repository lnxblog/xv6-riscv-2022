#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = 0;
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *filename)
{
    /*
    1. open path and check if dir
    2. get entries in the dir
    3. check if entry == file and is filename then break
    4. if directory go to step 2
    */
    struct dirent de;
    struct stat st;
    char buf[512], *p;
    int fd;
    char *curr_file;
    //printf("checking in %s for %s\n",path,filename);
    curr_file = fmtname(path);

    //
    if(!strcmp(filename,curr_file))
    {
      printf("%s\n", path);
    }


    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
      fprintf(2, "find: cannot stat %s\n", path);
      return;
    }
    switch(st.type){


      case T_DIR:
          if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
          }
          strcpy(buf, path);
          p = buf+strlen(buf);
          *p++ = '/';

          while(read(fd, &de, sizeof(de)) == sizeof(de)){

            if(de.inum == 0)
                continue;
          
            if(!strcmp(de.name,".") || !strcmp(de.name,".."))
            continue;


            memmove(p, de.name, DIRSIZ);
            //printf("next path %s\n",p);
            p[DIRSIZ] = 0;
            find(buf,filename);

          }
          break;
    }

    close(fd);

}



int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("find requires 3 args\n");
        return 1;
    }

    char *path,*filename;
    path = argv[1];
    filename = argv[2];

    find(path,filename);

  return 0; 
}
