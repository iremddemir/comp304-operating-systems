#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
  int pid;
  char *command_name = argv[1];
	
  pid = fork();

	// child process
	if(pid == 0) {
    //immediatly becomes zombie process
		exit(0);

	//parent
	} else if (pid > 0) {
		//sleep for 6s
    sleep(6); 

  //pid < 0 -> failed fork    
	} else {
    printf("Fork failed");
    return 1;
	}
	return 0;
}