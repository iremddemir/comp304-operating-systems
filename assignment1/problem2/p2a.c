#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
int main(int argc, char** argv) {
  int car_no = 1;
  int read_msg_b;
  int read_msg_c;
  int read_msg_d;
  int ord_pipe_ab[2];
  int ord_pipe_da[2];
  int ord_pipe_bc[2];
  int ord_pipe_cd[2];
  pid_t pid_b,pid_c,pid_d;

  //create pipe
  if(pipe(ord_pipe_ab)==-1)
  {
    fprintf(stderr,"Pipe A to B failed");
  }
  if(pipe(ord_pipe_da)==-1)
  {
    fprintf(stderr,"Pipe D to A failed");
  }
  //create Child B
  pid_b = fork();
  if (pid_b==-1){
    fprintf(stderr,"Fork Failed");
    return 1;
  }
  //Parent Process A  
  else if (pid_b > 0){
    printf("Started washing car %d\n",car_no);
    close(ord_pipe_ab[READ_END]);
    //send message ->B
    write(ord_pipe_ab[WRITE_END],&car_no,2);
    //get message from D
    close(ord_pipe_da[WRITE_END]);
    read(ord_pipe_da[READ_END], &read_msg_d,sizeof(read_msg_d));
    if (read_msg_d == 1){
      printf("Finished washing car %d\n", read_msg_d);
    }
    else{
      printf("there is some error encountered during wash pipeline");
    }
    
    exit(0);
  }
  //child B
  else{
    close(ord_pipe_ab[WRITE_END]);
    //get message from A
   close(ord_pipe_ab[WRITE_END]);
   read(ord_pipe_ab[READ_END],&read_msg_b,sizeof(read_msg_b));
    sleep(4);
    printf("Washing the windows of car %d\n",read_msg_b);
    //create pipe from B to C
    if(pipe(ord_pipe_bc) == -1){
      fprintf(stderr,"Pipe B to C Failed");
      return 1;
    }
    
    //create Child C
    pid_c = fork();
    if (pid_c==-1){
    fprintf(stderr,"Fork Failed");
    return 1;
    }
    //Parent B
    else if(pid_c>0){
      close(ord_pipe_bc[READ_END]);
      //send (readed message by b) message -> C 
      write(ord_pipe_bc[WRITE_END],&read_msg_b,sizeof(read_msg_b));
    }
    //Child C
    else{
      close(ord_pipe_bc[WRITE_END]);
      read(ord_pipe_bc[READ_END],&read_msg_c,sizeof(read_msg_c));
      sleep(4);
      printf("Washing the interior of car %d\n",read_msg_c);
      if(pipe(ord_pipe_cd) == -1){
      fprintf(stderr,"Pipe C to D Failed");
      return 1;
      }
      //Create Child D
      pid_d = fork();
      if (pid_d==-1){
        fprintf(stderr,"Fork Failed");
        return 1;
      }
      //Parent C
      else if(pid_d>0){
        close(ord_pipe_cd[READ_END]);
        //send (readed message by b) message -> C 
        write(ord_pipe_cd[WRITE_END],&read_msg_c,sizeof(read_msg_b));
      }
      //Child D
      else{
        close(ord_pipe_cd[WRITE_END]);
        read(ord_pipe_cd[READ_END],&read_msg_d,sizeof(read_msg_d));
        sleep(4);
        printf("Washing the wheels of car %d\n",read_msg_d);

        close(ord_pipe_da[READ_END]);
        //send message -> A
        write(ord_pipe_da[WRITE_END],&read_msg_d,sizeof(read_msg_b));
      }
    }
  }
  
  return 0;
}