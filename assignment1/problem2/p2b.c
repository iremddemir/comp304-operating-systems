#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#define READ_END 0
#define WRITE_END 1
#define FOR 999
int main(int argc, char** argv) {
  //take n as an command line argument
  int n = atoi(argv[1]);
  int read_msg_b;
  int read_msg_c;
  int read_msg_d;
  int ord_pipe_ab[2];
  int ord_pipe_da[2];
  int ord_pipe_bc[2];
  int ord_pipe_cd[2];
  int ord_pipe_btwn_loop[2];
  pid_t pid_b,pid_c,pid_d;
  //create pipe
  if(pipe(ord_pipe_ab)==-1)
  {
    fprintf(stderr,"Pipe A to B failed");
    return 1;
  }
  if(pipe(ord_pipe_da)==-1)
  {
    fprintf(stderr,"Pipe D to A failed");
    return 1;
  }
  //create pipe from B to C
  if(pipe(ord_pipe_bc) == -1){
    fprintf(stderr,"Pipe B to C Failed");
    return 1;
  }
  if(pipe(ord_pipe_cd) == -1){
    fprintf(stderr,"Pipe C to D Failed");
    return 1;
  }
  if(pipe(ord_pipe_btwn_loop) == -1){
    fprintf(stderr,"Pipe between loops failed");
    return 1;
  }

  for (int i = 0; i <FOR; i++){
  int car_no;
  //create Child B
  pid_b = fork();
  if (pid_b==-1){
    fprintf(stderr,"Fork Failed");
    return 1;
  }
  //Parent Process A  
  else if (pid_b > 0){
    if (i == 0){
      car_no = 1;
      printf("Started washing car %d\n",car_no);
      //send message ->B
      write(ord_pipe_ab[WRITE_END],&car_no,sizeof(car_no));
    }
    else{
    printf("Started washing car %d\n",car_no);
    //send message ->B
    read(ord_pipe_btwn_loop[READ_END], &car_no, sizeof(car_no));
    write(ord_pipe_ab[WRITE_END],&car_no,sizeof(car_no));
    }
  }
  //child B
  else{
    //get message from A
    read(ord_pipe_ab[READ_END],&read_msg_b,sizeof(read_msg_b));
    sleep(4);
    printf("Washing the windows of car %d\n",read_msg_b);
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
      read(ord_pipe_bc[READ_END],&read_msg_c,sizeof(read_msg_c));
      sleep(4);
      printf("Washing the interior of car %d\n",read_msg_c);
      //Create Child D
      pid_d = fork();
      if (pid_d==-1){
        fprintf(stderr,"Fork Failed");
        return 1;
      }
      //Parent C
      else if(pid_d>0){
        //send (readed message by b) message -> C 
        write(ord_pipe_cd[WRITE_END],&read_msg_c,sizeof(read_msg_b));
      }
      //Child D
      else{
        read(ord_pipe_cd[READ_END],&read_msg_d,sizeof(read_msg_d));
        sleep(4);
        printf("Washing the wheels of car %d\n",read_msg_d);
        //send message -> A
        write(ord_pipe_da[WRITE_END],&read_msg_d,sizeof(read_msg_b));
        exit(0);
      }
      wait(NULL);
      exit(0);
    }
     wait(NULL);
    exit(0);
  }
  read(ord_pipe_da[READ_END],&car_no,sizeof(car_no));
  printf("Finished washing car %d\n",car_no);
  car_no ++;
  if(car_no > n){
    exit(0);
  }
  write(ord_pipe_btwn_loop[WRITE_END],&car_no, sizeof(car_no));
  }
  
  return 0;
}