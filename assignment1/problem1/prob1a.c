#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char** argv) {
  
  //take n as an command line argument
  int n = atoi(argv[1]);
  //initialize level as 0 
  int lvl = 0;
  //store parent and child id's in the following variables
  int pid;
  int ppid;
 
  //base level
  pid = getpid();
  printf("Base Process ID: %d, level: %d\n", pid, lvl);
  //loop 
  for (int i = 0; i < n; i++){
    //create a child process
    pid = fork();
    //child process
    if (pid == 0){
      pid = getpid();
      ppid = getppid();
      //inside of the child so level is up
      lvl++;
      printf("Process ID: %d, Parent ID: %d, level: %d\n", pid, ppid, lvl);
    }
    //parent 
    else if (pid > 0){
      //then wait for a child to change its state
      wait(NULL);
    }
    //pid < 0: error
    else{
      printf("Fork failed");
      return 1;
    }
  }

  return 0;
}