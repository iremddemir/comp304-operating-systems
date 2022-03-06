#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#define READ_END 0
#define WRITE_END 1
int main(int argc, char** argv) {
  struct timeval msg_start_time_b;
  struct timeval msg_from_b;
  struct timeval msg_end;
  char *command_name = argv[1]; 
  int ord_pipe_ab[2];
  int ord_pipe_ba[2];
  //create pipes
  if(pipe(ord_pipe_ab)==-1)
  {
    fprintf(stderr,"Pipe A to B failed");
  }
  if(pipe(ord_pipe_ba)==-1)
  {
    fprintf(stderr,"Pipe B to A failed");
  }
  
  pid_t pid_b = fork();
  if (pid_b < 0){
    fprintf(stderr,"Fork Failed");
    return 1;
  }
  //Child B 
  else if (pid_b == 0){
    gettimeofday(&msg_from_b, NULL);
		write(ord_pipe_ba[WRITE_END], &msg_from_b, sizeof(msg_from_b)); 
    execvp(command_name, &argv[1]);
    
  }
  //Parent A
  else{
    wait(NULL);
    read(ord_pipe_ba[READ_END],&msg_start_time_b,sizeof(msg_start_time_b));
    gettimeofday(&msg_end, NULL);
    
    printf("B started at %d:%0d:%0d\n",
            localtime(&msg_start_time_b.tv_sec)->tm_hour,
            localtime(&msg_start_time_b.tv_sec)->tm_min,
            localtime(&msg_start_time_b.tv_sec)->tm_sec
          );
       printf("It ended at %d:%0d:%0d\n",
            localtime(&msg_end.tv_sec)->tm_hour,
            localtime(&msg_end.tv_sec)->tm_min,
            localtime(&msg_end.tv_sec)->tm_sec
          );
    struct timeval time_difference;
    timersub(&msg_end, &msg_start_time_b, &time_difference);

    printf("Elapsed time: %d:%0d:%0d\n",
            localtime(&time_difference.tv_sec)->tm_hour,
            localtime(&time_difference.tv_sec)->tm_min,
            localtime(&time_difference.tv_sec)->tm_sec);
  }
  
  return 0;
}