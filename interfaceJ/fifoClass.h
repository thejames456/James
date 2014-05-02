#ifndef fifoClass_H
#define fifoClass_H

#include <string>
#include "interfaceJ.h"

class fifoClass
{
private:
	std::string path;
	interfaceJ* intfj;
	
public:
	fifoClass();
	//create a fifo and set path
	fifoClass(interfaceJ* ij, std::string jobid, std::string dir);
	
	//return path
	std::string getPath();
	
	//open and read from the fifo until it is closed
	void readReports();
};


#endif
