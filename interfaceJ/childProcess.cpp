#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include "childProcess.h"

#include <signal.h>

int childProcess::start(char* file, std::vector<char*> args, char* workingDir)
{
	int outPipe[2];
	
	if(pipe(outPipe) < 0)//create pipe for reading stdout and stderr
		return -1;
	
	child = fork();
	if(child == -1) //fork failed
		return -2;
	else if(child == 0)//child process
	{
		//build args
		args.insert(args.begin(), file);
		args.push_back(NULL);
		
		//copy vector into c array
		int size = args.size();
		char** execArgs = new char*[size];
		for(int i = 0; i < args.size(); i++)
		{
			execArgs[i] = args[i];
		}
		
		//change working directory
		if(workingDir != NULL)
			chdir(workingDir);
		
		//redirect stdout and stderr
		if(dup2(outPipe[1], STDOUT_FILENO) == -1)
			return -1;
		if(dup2(outPipe[1], STDERR_FILENO) == -1)
			return -1;
		
		close(outPipe[0]);
		
		//call exec
		execv(file, execArgs);

		//if exec fails
		return -1;
	}
	else//parent process
	{
		close(outPipe[1]);
		
		outputPipe = outPipe[0];
		_eof = false;
		
		return 0;
	}
	
}

bool childProcess::hasExited()
{
	int status;
	pid_t r = waitpid(child, &status, WNOHANG);

	return (r != 0);
}
int childProcess::waitForExit()
{
	int status = -1;
	waitpid(child, &status, 0);
	if(WIFEXITED(status))
		return WEXITSTATUS(status);
	else
		return -1;
}

bool childProcess::eof()
{
	return _eof;
}

std::string childProcess::getLine()
{
	char c;
	ssize_t s;
	
	std::string str;

	while(true)
	{
		s = read(outputPipe, &c, 1);
		if(s != 1)
		{
			_eof = true;
			break;
		}
		if(c == '\n')
			break;
		str += c;
	}
	return str;
}

void childProcess::kill_the_child()
{
	kill(child, SIGKILL);
}


