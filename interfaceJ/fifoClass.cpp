#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include "interfaceJ.h"
#include "fifoClass.h"
#include <errno.h>
fifoClass::fifoClass()
{
}
fifoClass::fifoClass(interfaceJ* ij, std::string jobid, std::string dir)
{
	intfj = ij;
	
	std::stringstream ss;
	ss << dir << "intfJfifo" << jobid;
	path = ss.str();
	
	mode_t perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP;
	
	mode_t oldMask = umask(0);
	int result = mkfifo(path.c_str(), perms);
	umask(oldMask);

	std::cout << "fifo: " << path << "\n";	
	std::cout << "mkfifo: " << result << "\n";
	if(result == -1)
		std::cout << "errno: " << errno << "\n";
}

//return path
std::string fifoClass::getPath()
{
	return path;
}




//open and read from the fifo until it is closed
void fifoClass::readReports()
{
	int fd = open(path.c_str(), O_RDONLY);
	if(fd == -1)
	{
		std::cout << "open failed\n";
		return;
	}
	std::cout << "fifo opened\n";
	
	char op;
	char ch;
	ssize_t amt;
	while(true)
	{
		amt = read(fd, &op, 1);
		if(amt <= 0)
			break;
		
		if(op == 0) // read progress
		{
			amt = read(fd, &ch, 1);
			if(amt <= 0)
				break;
			int percent = (unsigned char)ch;
			
			intfj->reportProgress(percent);
		}
		else if(op >= 1 && op <= 3) // read string
		{
			std::string str;
			while(true)
			{
				amt = read(fd, &ch, 1);
				if(amt <= 0)
					break;
				if(ch == 0)
					break;
				str += ch;
			}
			if(amt <= 0)
				break;
			
			if(op == 1) // saved image
				intfj->reportSavedFile(str);
			else if(op == 2) // saved data
				intfj->reportSavedFile(str);
			else if(op == 3) // error
				intfj->reportError(str);
		}
		else
		{
			std::cout << "unknown op\n";
			break;
		}
	}
	
	if(amt == -1)
		std::cout << "read error\n";
	
	close(fd);
	std::cout << "fifo closed\n";
	unlink(path.c_str());
	std::cout << "fifo unlinked\n";
}

