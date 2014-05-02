#ifndef childProc_H
#define childProc_H

#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
//maybe this for the library. 
#include <signal.h>

class childProcess
{
private:
	pid_t child;
	int outputPipe;
	bool _eof;
public:
	int start(char* file, std::vector<char*> args, char* workingDir);
	
	bool hasExited();
	int waitForExit();
	
	void kill_the_child();
	
	bool eof();
	std::string getLine();
	
};

#endif