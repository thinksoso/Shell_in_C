#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h> 


char* read_line(){
	char* line = (char*)malloc(sizeof(char)*100);
	fgets(line,100,stdin);
	line[strlen(line)-1]='\0';
	return line;
}

char** split_line(char* line){
	char** args = (char**)malloc(sizeof(char*)*100);
	int len = strlen(line);
	int arg_num = 0;
	int char_num = 0;
	char* arg = (char*)malloc(sizeof(char)*100);
	for(int i=0;i<len;i++){
//		printf("%c\n",line[i]);
		if(line[i]==' '){
			arg[char_num]='\0';
			args[arg_num] = arg;
			char_num = 0;
			arg_num++;
			arg = (char*)malloc(sizeof(char)*100); 
			continue;
		}
		arg[char_num] = line[i];
		char_num++;
	}
	arg[char_num]='\0';
	args[arg_num]=arg;
	return args;
}

int msh_cd(char** args){
	if(chdir(args[1])!=0) {
		printf("cd error");
		return -1;
	}
	return 0;
}

int msh_help(char** args){
	printf("a toy shell!\n");
	printf("input exit to close the shell\n");
	return 0;
}

int msh_command(char** args){
	// use fork() to create a child process, use exec() to execuate new program in this process.
	pid_t pid,wpid;
	int status;
	pid = fork();
	if(pid == 0){
		//child process
		if(execvp(args[0],args) == -1){
			printf("%s error",args[0]);
			return -1;
		}
	}
	else if(pid < 0){
		printf("fork error\n");
		return -1;
	}
	else{
		do{
			 wpid = waitpid(pid, &status, WUNTRACED);
		}
		while ( !WIFSIGNALED(status) && !WIFEXITED(status) );
	}
	return 0;
}
				


int msh_execute(char** args){
	int status = 0;
	if(!strcmp(args[0],"cd")) status = msh_cd(args);
	else if(!strcmp(args[0],"help")) status = msh_help(args);
	else if(!strcmp(args[0],"exit")) status = 1;
	else status = msh_command(args);
	return status;		
}
	

int msh_loop(){
	char* line;
	char** args;
	int status = 0;
	while(!status){
		printf(">");
		line = read_line();
		args = split_line(line);
		status = msh_execute(args);
		free(line);
		free(args);
	}
	return 0;
}

int main(){

	msh_loop();

	return 0;
}
