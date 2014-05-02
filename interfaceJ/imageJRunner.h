#ifndef ijRunner_H
#define ijRunner_H

#include "childProcess.h"
#include <string>

class imageJRunner
{
private:
	childProcess proc;
	std::string imageJPath;
	
	bool useIntfJ;
	std::string inputImage;
	std::string outputPath;
	std::string fifoPath;
	
	int port;

public:
	

	imageJRunner();	
	
	void setImageJPath(std::string path);
	void enableInterfaceJ();
	void setInputImage(std::string file);
	void setOutputPath(std::string path);
	void setFifoPath(std::string path);
	void setPort(int p);
	int runImageJWithMacro(std::string file, bool xvfb);
	
	std::string readOutputLine();
	std::string readOutputToEnd();
	void readOutputToStdout();
	bool eof();
	
	int waitForExit();
	int getExitCode();
	bool hasExited();
	
	void kill_children();	
};

#endif
