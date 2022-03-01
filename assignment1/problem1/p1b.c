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
    execvp(command_name, &argv[1]);
		return 1;

	//parent
	} else if (pid > 0) {
		//then wait for a child to change its state
    wait(NULL); 
    //print as given in the instructions
    printf("Child finished executing %s\n", command_name);

  //pid < 0 -> failed fork    
	} else {
    printf("Fork failed");
    return 1;
	}
	return 0;
}