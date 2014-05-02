#ifndef intfj_H
#define intfj_H

#include <string>
#include <vector>

#include "imageJRunner.h"
#include "ImageJLog.h"

class interfaceJ;
#include "fifoClass.h"

enum interfaceJReportType
{
	REPORT_PROGRESS,
	REPORT_ERROR,
	REPORT_FINISHED
};

typedef void (*report_callback)(interfaceJReportType type, std::vector<std::string> data, void* user_object);


class interfaceJ
{
private:
	imageJRunner imageJ;
	fifoClass fifo;
	
	std::string jobId;
	std::string image;
	std::string macro;
	std::string outputdir;
	std::string imageJPath;
	void* user;
	
	report_callback callback;
	
	//int progress;
	std::string errorMsg;
	std::vector<std::string> savedFiles;
	bool macroComplete;
	
public:
	interfaceJ();
	
	void setJobId(std::string id);
	void setImagePath(std::string path);
	void setMacroText(std::string text);
	void setOutputDir(std::string path);
	void setImageJDir(std::string path);
	void setUserObject(void* obj);
	void setCallback(report_callback func);
	
	int runImageJ();
	int kill();
	
	
	
	//used by fifo
	void reportProgress(int percent);
	void reportSavedFile(std::string file);
	void reportError(std::string message);
	void reportFinished();
	
};






#endif
